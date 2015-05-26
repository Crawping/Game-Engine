// Header Files
//=============

#include "CollisionMeshExporter.h"

#include <cstdint>
#include <fstream>
#include <map>
#include <maya/MDagPath.h>
#include <maya/MFloatVector.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFnMesh.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MItDag.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItSelectionList.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h >
#include <maya/MPlugArray.h >
#include <maya/MObjectArray.h >
#include <maya/MPlug.h>

#include <sstream>
#include <string>


//Globals
MString name;
std::vector<float> g_OctreeNodes[4][4][4];
float g_octreeStart = -2000.0f;
float g_octreeEnd = 2000.0f;
int g_numberOfSplits = 4;
int size = 0;
float g_octreeSectorWidth = (g_octreeEnd - g_octreeStart) / g_numberOfSplits;

// Helper Functions
float minValue(float i_value1, float i_value2, float i_value3)
{
	return (i_value1 < i_value2) ? ((i_value1 < i_value3) ? i_value1 : i_value3) : ((i_value2 < i_value3) ? i_value2 : i_value3);
}

float maxValue(float i_value1, float i_value2, float i_value3)
{
	return (i_value1 > i_value2) ? ((i_value1 > i_value3) ? i_value1 : i_value3) : ((i_value2 > i_value3) ? i_value2 : i_value3);
}
// Vertex Definition
//==================

namespace
{
	struct sVertex
	{
		// Position
		float x, y, z;
		// Normal
		float nx, ny, nz;
		// Tangent
		float tx, ty, tz;
		// Bitangent
		float btx, bty, btz;
		// Texture coordinates
		float u, v;
		// Color
		// (Note that each color channel is a float and [0,1], _not_ a uint8_t [0,255]!)
		float r, g, b, a;

		sVertex(const MPoint& i_position, const MFloatVector& i_normal,
			const MFloatVector& i_tangent, const MFloatVector& i_bitangent,
			const float i_texcoordU, const float i_texcoordV,
			const MColor& i_vertexColor)
			:
			x(static_cast<float>(i_position.x)), y(static_cast<float>(i_position.y)), z(static_cast<float>(i_position.z)),
			nx(i_normal.x), ny(i_normal.y), nz(i_normal.z),
			tx(i_tangent.x), ty(i_tangent.y), tz(i_tangent.z),
			btx(i_bitangent.x), bty(i_bitangent.y), btz(i_bitangent.z),
			u(i_texcoordU), v(i_texcoordV),
			r(i_vertexColor.r), g(i_vertexColor.g), b(i_vertexColor.b), a(i_vertexColor.a)
		{

		}
	};
}

// Helper Function Declarations
//=============================

namespace
{
	std::string CreateUniqueVertexKey(const int i_positionIndex, const int i_normalIndex,
		const int i_tangentIndex, const int i_texcoordIndex, const int i_vertexColorIndex);
	MStatus ProcessAllMeshes(std::vector<float>& o_vertexBuffer, std::vector<unsigned int>& o_indexBuffer, MFileObject i_filePath, std::string i_meshNames, std::vector<std::string>& i_materialNames);
	MStatus ProcessSelectedMeshes(std::vector<float>& o_vertexBuffer, std::vector<unsigned int>& o_indexBuffer, MFileObject i_filePath, std::string i_meshNames, std::vector<std::string>& i_materialNames);
	MStatus ProcessSingleMesh(const MFnMesh& i_mesh,
		std::vector<float>& o_vertexBuffer, std::vector<unsigned int>& o_indexBuffer, std::string i_meshNames, std::vector<std::string>& i_materialNames);
	MStatus FillVertexAndIndexBuffer(const MFnMesh& i_mesh,
		std::vector<float>& o_vertexBuffer, std::vector<unsigned int>& o_indexBuffer);
	MStatus WriteMeshToFile(MFileObject i_fileName, std::vector<float>& i_vertexBuffer, std::vector<unsigned int>& i_indexBuffer);
}

// Inherited Interface
//====================
void WriteMeshAndMaterialNamesToFile(MFileObject i_filePath, std::string i_meshNames, std::vector<std::string>& i_materialNames)
{
	std::string meshFile = "meshList.txt";
	i_filePath.setRawName(meshFile.c_str());

	std::ofstream meshout(i_filePath.fullName().asChar());
	if (meshout.is_open())
	{
		meshout << "return\n{\n";
		for (unsigned int i = 0; i < i_meshNames.size(); i++)
		{
			meshout << "\"" << i_meshNames[i] << "\"" << "," << "\n";
		}
		meshout << "}\n";
		meshout.close();
	}

	std::string meshesToBuildFile = "meshesToBuild.txt";
	i_filePath.setRawName(meshesToBuildFile.c_str());
	std::ofstream meshBuildNames(i_filePath.fullName().asChar());
	if (meshBuildNames.is_open())
	{
		meshBuildNames << "return\n{\n";
		meshBuildNames << "{\n";
		meshBuildNames << "builder = \"MeshBuilder.exe\",\n";
		meshBuildNames << "extensions =\n{\nsource = \"txt\",\ntarget = \"txt\",\n},";
		meshBuildNames << "assets =\n{";

		for (unsigned int i = 0; i < i_meshNames.size(); i++)
		{
			meshBuildNames << "{\nname = \"Level/" << i_meshNames[i] << "\"," << "\n},";
		}
		meshBuildNames << "},\n},\n}";
		meshBuildNames.close();

	}

	std::string sceneFile = "sceneFile.txt";
	i_filePath.setRawName(sceneFile.c_str());
	std::ofstream sceneFileOutput(i_filePath.fullName().asChar());
	if (sceneFileOutput.is_open())
	{
		sceneFileOutput << "return\n{\n";
		for (unsigned int i = 0; i < i_meshNames.size(); i++)
		{
			sceneFileOutput << "{\n";
			sceneFileOutput << "name=" << "\"" << i_meshNames[i] << "\",\n";
			sceneFileOutput << "controller=" << "\"NullController\",\n";
			sceneFileOutput << "position=" << "{0.0,0.0,0.0},\n";
			sceneFileOutput << "material=" << "\"" << i_materialNames[i] << "\",\n";
			sceneFileOutput << "mesh=" << "\"" << i_meshNames[i] << "\",\n";
			sceneFileOutput << "},\n";
		}

		sceneFileOutput << "}";
		sceneFileOutput.close();

	}
}

MStatus eae6320::CollisionMeshExporter::writer(const MFileObject& i_file, const MString& i_options, FileAccessMode i_mode)
{
	// Gather the vertex and index buffer information
	std::vector<float> vertexBuffer;
	std::vector<unsigned int> indexBuffer;
	std::string meshNames;
	std::vector<std::string> materialNames;
	MStatus status;
	{
		const MString filePath = i_file.fullName();
		std::string fileName = filePath.asChar();
		fileName = fileName.substr(0, fileName.length() - 4);
		// The user decides whether to export the entire scene or just a selection
		if (i_mode == MPxFileTranslator::kExportAccessMode)
		{
			status = ProcessAllMeshes(vertexBuffer, indexBuffer, i_file, meshNames, materialNames);
			if (!status)
			{
				return status;
			}
		}
		else if (i_mode == MPxFileTranslator::kExportActiveAccessMode)
		{
			cout << "SelectedMesh+"<<i_file.fullName().asChar();
			status = ProcessSelectedMeshes(vertexBuffer, indexBuffer, i_file, meshNames, materialNames);
			if (!status)
			{
				return status;
			}
		}
		else
		{
			MGlobal::displayError("Unexpected file access mode");
			return MStatus::kFailure;
		}

		WriteMeshToFile(i_file, vertexBuffer, indexBuffer);

		return status;
	}
}

// Helper Function Definitions
//============================

namespace
{
	std::string CreateUniqueVertexKey(const int i_positionIndex, const int i_normalIndex,
		const int i_tangentIndex, const int i_texcoordIndex, const int i_vertexColorIndex)
	{
		std::ostringstream vertexKey;
		vertexKey << i_positionIndex << "_" << i_normalIndex << "_"
			<< i_tangentIndex << "_" << i_texcoordIndex << "_" << i_vertexColorIndex;
		return vertexKey.str();
	}

	MStatus ProcessAllMeshes(std::vector<float>& o_vertexBuffer, std::vector<unsigned int>& o_indexBuffer, MFileObject i_filePath, std::string i_meshNames, std::vector<std::string>& i_materialNames)
	{
		for (MItDag i(MItDag::kDepthFirst, MFn::kMesh); !i.isDone(); i.next())
		{
			MFnMesh mesh(i.item());
			if (!ProcessSingleMesh(mesh, o_vertexBuffer, o_indexBuffer, i_meshNames, i_materialNames))
			{
				return MStatus::kFailure;
			}
		}

		return MStatus::kSuccess;
	}

	MStatus ProcessSelectedMeshes(std::vector<float>& o_vertexBuffer, std::vector<unsigned int>& o_indexBuffer, MFileObject i_filePath,std::string i_meshNames, std::vector<std::string>& i_materialNames)
	{
		// Iterate through each selected mesh
		MSelectionList selectionList;
		MStatus status = MGlobal::getActiveSelectionList(selectionList);
		if (status)
		{
			for (MItSelectionList i(selectionList, MFn::kMesh); !i.isDone(); i.next())
			{
				MDagPath dagPath;
				i.getDagPath(dagPath);
				MFnMesh mesh(dagPath);
				if (!ProcessSingleMesh(mesh, o_vertexBuffer, o_indexBuffer, i_meshNames, i_materialNames))
				{
					return MStatus::kFailure;
				}
				//cout << "WritingMesh";
			}
		}
		else
		{
			MGlobal::displayError(MString("Failed to get active selection list: ") + status.errorString());
			return MStatus::kFailure;
		}

		return MStatus::kSuccess;
	}



	MString GetShaderName(MObject shadingEngine)
	{
		// attach a function set to the shading engine
		MFnDependencyNode fn(shadingEngine);

		// get access to the surfaceShader attribute. This will be connected to
		// lambert , phong nodes etc.
		MPlug sshader = fn.findPlug("surfaceShader");

		// will hold the connections to the surfaceShader attribute
		MPlugArray materials;

		// get the material connected to the surface shader
		sshader.connectedTo(materials, true, false);

		// if we found a material
		if (materials.length())
		{
			MFnDependencyNode fnMat(materials[0].node());
			return fnMat.name();
		}
		return "none";
	}

	void OutputMeshInstances(MObject mesh, std::string i_meshNames, std::vector<std::string>& i_materialNames)
	{
		MFnMesh fnMesh(mesh);

		// get the number of instances
		int NumInstances = fnMesh.parentCount();


		// loop through each instance of the mesh
		for (int i = 0; i< NumInstances; ++i)
		{
			// attach a function set to this instances parent transform
			MFnDependencyNode fn(fnMesh.parent(i));


			// this will hold references to the shaders used on the meshes
			MObjectArray Shaders;

			// this is used to hold indices to the materials returned in the object array
			MIntArray    FaceIndices;

			// get the shaders used by the i'th mesh instance
			fnMesh.getConnectedShaders(i, Shaders, FaceIndices);

			switch (Shaders.length()) {

				// if no shader applied to the mesh instance
			case 0:
			{
					  i_materialNames.push_back("None,0");;
			}
				break;

				// if all faces use the same material
			case 1:
			{
					  i_materialNames.push_back(GetShaderName(Shaders[0]).asChar());
			}
				break;

				// if more than one material is used, write out the face indices the materials
				// are applied to.
			default:
			{
					   i_materialNames.push_back("default");
			}
				break;
			}
		}
	}

	MStatus ProcessSingleMesh(const MFnMesh& i_mesh,
		std::vector<float>& o_vertexBuffer, std::vector<unsigned int>& o_indexBuffer, std::string i_meshNames, std::vector<std::string>& i_materialNames)
	{
		if (i_mesh.isIntermediateObject())
		{
			return MStatus::kSuccess;
		}
		OutputMeshInstances(i_mesh.object(), i_meshNames, i_materialNames);
		return FillVertexAndIndexBuffer(i_mesh, o_vertexBuffer, o_indexBuffer);
	}

	MStatus FillVertexAndIndexBuffer(const MFnMesh& i_mesh,
		std::vector<float>& o_vertexBuffer, std::vector<unsigned int>& o_indexBuffer)
	{
		MStatus status;

		// Get a list of the positions
		MPointArray positions;
		{
			status = i_mesh.getPoints(positions);
			if (!status)
			{
				MGlobal::displayError(status.errorString());
				return status;
			}
		}

		// Get a list of the normals
		MFloatVectorArray normals;
		{
			status = i_mesh.getNormals(normals);
			if (!status)
			{
				MGlobal::displayError(status.errorString());
				return status;
			}
		}

		// Get a list of tangents
		MFloatVectorArray tangents;
		{
			status = i_mesh.getTangents(tangents);
			if (!status)
			{
				MGlobal::displayError(status.errorString());
				return status;
			}
		}

		// Get a list of bitangents
		MFloatVectorArray bitangents;
		{
			status = i_mesh.getBinormals(bitangents);
			if (!status)
			{
				MGlobal::displayError(status.errorString());
				return status;
			}
		}

		// Get a list of the texture coordinates
		MFloatArray texcoordUs, texcoordVs;
		{
			status = i_mesh.getUVs(texcoordUs, texcoordVs);
			if (!status)
			{
				MGlobal::displayError(status.errorString());
				return status;
			}
		}

		// Get a list of the vertex colors
		MColorArray vertexColors;
		{
			int colorSetCount = i_mesh.numColorSets();
			if (colorSetCount > 0)
			{
				MString* useDefaultColorSet = NULL;
				MColor defaultColor(1.0f, 1.0f, 1.0f, 1.0f);
				status = i_mesh.getColors(vertexColors, useDefaultColorSet, &defaultColor);
				if (!status)
				{
					MGlobal::displayError(status.errorString());
					return status;
				}
			}
		}

		// Gather vertex and triangle information
		std::map<const std::string, const sVertex> uniqueVertices;
		std::vector<const std::string> triangles;
		{
			MPointArray trianglePositions;
			MIntArray positionIndices;
			for (MItMeshPolygon i(i_mesh.object()); !i.isDone(); i.next())
			{
				if (i.hasValidTriangulation())
				{
					// Store information for each vertex in the polygon
					std::map<int, const std::string> indexToKeyMap;
					{
						MIntArray vertices;
						status = i.getVertices(vertices);
						if (status)
						{
							for (unsigned int j = 0; j < vertices.length(); ++j)
							{
								const int positionIndex = vertices[j];
								const int normalIndex = i.normalIndex(j);
								const int tangentIndex = i.tangentIndex(j);
								int texcoordIndex;
								{
									status = i.getUVIndex(j, texcoordIndex);
									if (!status)
									{
										MGlobal::displayError(status.errorString());
										return status;
									}
								}
								int vertexColorIndex = -1;
								MColor vertexColor(1.0f, 1.0f, 1.0f, 1.0f);
								{
									int colorSetCount = i_mesh.numColorSets();
									if (colorSetCount > 0)
									{
										status = i.getColorIndex(j, vertexColorIndex);
										if (status)
										{
											vertexColor = vertexColors[vertexColorIndex];
										}
										else
										{
											MGlobal::displayError(status.errorString());
											return status;
										}
									}
								}
								const std::string vertexKey = CreateUniqueVertexKey(positionIndex, normalIndex,
									tangentIndex, texcoordIndex, vertexColorIndex);
								indexToKeyMap.insert(std::make_pair(positionIndex, vertexKey));
								uniqueVertices.insert(std::make_pair(vertexKey,
									sVertex(positions[positionIndex], normals[normalIndex],
									tangents[tangentIndex], bitangents[tangentIndex],
									texcoordUs[texcoordIndex], texcoordVs[texcoordIndex],
									vertexColor)));
							}
						}
						else
						{
							MGlobal::displayError(status.errorString());
							return status;
						}
					}
					// Store information for each individual triangle in the polygon
					{
						int triangleCount = 0;
						i.numTriangles(triangleCount);
						for (int j = 0; j < triangleCount; ++j)
						{
							i.getTriangle(j, trianglePositions, positionIndices);
							o_vertexBuffer.push_back(trianglePositions[0].x);
							o_vertexBuffer.push_back(trianglePositions[0].y);
							o_vertexBuffer.push_back(trianglePositions[0].z);

							o_vertexBuffer.push_back(trianglePositions[2].x);
							o_vertexBuffer.push_back(trianglePositions[2].y);
							o_vertexBuffer.push_back(trianglePositions[2].z);

							o_vertexBuffer.push_back(trianglePositions[1].x);
							o_vertexBuffer.push_back(trianglePositions[1].y);
							o_vertexBuffer.push_back(trianglePositions[1].z);


							//Decide which octree node you belong to
							//Find x sector
							float minAlongX = minValue(trianglePositions[0].x, trianglePositions[1].x, trianglePositions[2].x);
							float maxAlongX = maxValue(trianglePositions[0].x, trianglePositions[1].x, trianglePositions[2].x);
							
							int sectorStartX = (int)(minAlongX - g_octreeStart) / (int)g_octreeSectorWidth;
							int sectorEndX = (int)(maxAlongX - g_octreeStart) / (int)g_octreeSectorWidth;
							//cout << "MinAlongX= " << sectorStartX << endl;
							//cout << "MaxAlongX= " << sectorEndX << endl;
							for (int x = sectorStartX; x <= sectorEndX; x ++)
							{
								float minAlongY = minValue(trianglePositions[0].y, trianglePositions[1].y, trianglePositions[2].y);
								float maxAlongY = maxValue(trianglePositions[0].y, trianglePositions[1].y, trianglePositions[2].y);
							
								int sectorStartY = (int)(minAlongY - g_octreeStart) / (int)g_octreeSectorWidth;
								int sectorEndY = (int)(maxAlongY - g_octreeStart) / (int)g_octreeSectorWidth;
								//cout << "MinAlongY= " << sectorStartY << endl;
								//cout << "MaxAlongY= " << sectorEndY << endl;
								for (int y = sectorStartY; y <= sectorEndY; y ++)
								{
									float minAlongZ = minValue(-trianglePositions[0].z, -trianglePositions[1].z, -trianglePositions[2].z);
									float maxAlongZ = maxValue(-trianglePositions[0].z, -trianglePositions[1].z, -trianglePositions[2].z);
									int sectorStartZ = (int)(minAlongZ - g_octreeStart) / (int)g_octreeSectorWidth;
									int sectorEndZ = (int)(maxAlongZ - g_octreeStart) / (int)g_octreeSectorWidth;

									//cout << "MinAlongZ= " << sectorStartZ << endl;
									//cout << "MaxAlongZ= " << sectorEndZ << endl;
									for (int z = sectorStartZ; z <= sectorEndZ; z ++)
									{
										g_OctreeNodes[x][y][z].push_back(trianglePositions[0].x);
										g_OctreeNodes[x][y][z].push_back(trianglePositions[0].y);
										g_OctreeNodes[x][y][z].push_back(-trianglePositions[0].z);

										g_OctreeNodes[x][y][z].push_back(trianglePositions[2].x);
										g_OctreeNodes[x][y][z].push_back(trianglePositions[2].y);
										g_OctreeNodes[x][y][z].push_back(-trianglePositions[2].z);

										g_OctreeNodes[x][y][z].push_back(trianglePositions[1].x);
										g_OctreeNodes[x][y][z].push_back(trianglePositions[1].y);
										g_OctreeNodes[x][y][z].push_back(-trianglePositions[1].z);
										size++;
									}
																		
								}
							}
							for (unsigned int k = 0; k < positionIndices.length(); ++k)
							{
								const int positionIndex = positionIndices[k];
								std::map<int, const std::string>::iterator mapLookUp = indexToKeyMap.find(positionIndex);
								if (mapLookUp != indexToKeyMap.end())
								{
									const std::string& vertexKey = mapLookUp->second;
									triangles.push_back(vertexKey);
								}
								else
								{
									MGlobal::displayError("A triangle gave a different vertex index than the polygon gave");
									return MStatus::kFailure;
								}
							}
						}
					}
				}
				else
				{
					MGlobal::displayError("This mesh has an invalid triangulation");
					return MStatus::kFailure;
				}
			}
		}

		// Convert the triangle information to vertex and index buffers
	
		return MStatus::kSuccess;
	}


	MStatus WriteMeshToFile(MFileObject i_fileName, std::vector<float>& i_vertexBuffer, std::vector<unsigned int>& i_indexBuffer)
	{
		// Maya's coordinate system is different than the default Direct3D behavior
		// (it is right handed and UVs have (0,0) at the lower left corner).
		// You can deal with these differences however you want,
		// but if you want to convert everything in the exported file
		// the following are probably the most natural changes to make:
		//	* POSITION	-> x, y, -z
		//	* NORMAL	-> nx, ny, -z
		//	* TANGENT	-> tx, ty, -tz
		//	* BITANGENT	-> -btx, -bty, btz
		//	* TEXCOORD	-> u, 1 - v
		std::ofstream meshBuildNames(i_fileName.fullName().asChar());
		if (meshBuildNames.is_open())
		{
			meshBuildNames << "return\n{\n";
			meshBuildNames << "{\n";
			meshBuildNames << "builder = \"CollisionMeshBuilder.exe\",\n";
			meshBuildNames << "extensions =\n{\nsource = \"txt\",\ntarget = \"txt\",\n},";
			meshBuildNames << "assets =\n{";



		}
		cout << "SIZE" << size;
		for (int i = 0; i < g_numberOfSplits; i++)
		{
			

			for (int j = 0; j < g_numberOfSplits; j++)
			{
				for (int k= 0; k < g_numberOfSplits; k++)
				{
					i_fileName.setRawName((std::string("OctreeLeaf[") + std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k) + "]" + std::string(".txt")).c_str());
					std::ofstream fout(i_fileName.fullName().asChar());
					if (fout.is_open())
					{
						// Open table
						fout << "return\n{\n";
						{
							fout << "NumberOfVertices =" << g_OctreeNodes[i][j][k].size() / 3 << ",\n";
							// Writing vertex data table
							fout << "Vertices = \n{\n";
							meshBuildNames << "{\nname = \"StaticCollisionData/" << (std::string("OctreeLeaf[") + std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k) + "]") << "\"," << "\n},";

							for (unsigned int l = 0; l < g_OctreeNodes[i][j][k].size(); l += 3)
							{
								fout << "{" << g_OctreeNodes[i][j][k][l] << " , " << g_OctreeNodes[i][j][k][l + 1] << " , " << g_OctreeNodes[i][j][k][l + 2] << "},\n";
							}
						}
						// Close vertices table
						fout << "}\n";
						// Close table
						fout << "}\n";
					}
					else
					{
						MGlobal::displayError(MString("Couldn't open ") + MString(i_fileName.fullName().asChar()) + " for writing");
						return MStatus::kFailure;
					}
					fout.close();
				}
			}
		}

		meshBuildNames << "},\n},\n}";
		meshBuildNames.close();
		return MStatus::kSuccess;
	}
}
