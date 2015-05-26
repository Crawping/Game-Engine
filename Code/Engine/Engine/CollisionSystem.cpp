#include "CollisionSystem.h"
#include "Matrix4X4.h"
#include "Vector3.h"
#include "World.h"
#include "Utils.h"
#include "Timer.h"
std::vector<Engine::CollisionSystem> Engine::CollisionSystem::m_CollidingActors;

//Copy Constructor
Engine::CollisionSystem::CollisionSystem(const Engine::CollisionSystem& i_Collision)
{
	m_Collider=i_Collision.m_Collider;
	m_Actor=i_Collision.m_Actor;
	m_BitMask=i_Collision.m_BitMask;
	m_CollisionMask=i_Collision.m_CollisionMask;
	m_Info=i_Collision.m_Info;
}

//Assignment Operator
inline const Engine::CollisionSystem& Engine::CollisionSystem::operator=(const CollisionSystem& i_Collision)
{
	m_Collider=i_Collision.m_Collider;
	m_Actor=i_Collision.m_Actor;
	m_BitMask=i_Collision.m_BitMask;
	m_CollisionMask=i_Collision.m_CollisionMask;
	m_Info=i_Collision.m_Info;
	return *this;
}

//Delete Actor From Vector Of Colliding Actors
void Engine::CollisionSystem::deleteActor(const unsigned int i_Index)
{
	Engine::CollisionSystem temp=m_CollidingActors[i_Index];
	m_CollidingActors[i_Index]=m_CollidingActors[m_CollidingActors.size()-1];
	m_CollidingActors[m_CollidingActors.size()-1]=temp;
	m_CollidingActors.pop_back();
}

//Clear the Vector Of Colliding Actors
void Engine::CollisionSystem::deleteActors()
{
	while(m_CollidingActors.size()!=0)
	{
		m_CollidingActors.pop_back();
	}
	std::vector <CollisionSystem>().swap(m_CollidingActors);
}

//Check For Actor Destruction
void Engine::CollisionSystem::checkForDeath()
{
	unsigned int i=0;
	while(i<m_CollidingActors.size())
	{
		if(m_CollidingActors[i].m_Actor->getMarkedForDeath())
		{
			deleteActor(i);
		}
		else
			i++;
	}
}

Engine::CollisionInformation& Engine::CollisionSystem::getCollisionInfo()
{
	return m_Info;
}


//Detect Collisions
float Engine::CollisionSystem::checkForCollisions(const float i_FrameTime)
{
	std::vector<CollisionSystem> SingleCollisions;
	bool check[500][500];
	for(unsigned int i=0;i<500;i++)
	{
		for(unsigned int j=0;j<500;j++)
			check[i][j]=true;
	}	

	//Loop through the Actors in the Vector Of Colliding Actors and Perform Sweep Tests to Check for Collisions
	for(unsigned int i=0;i<m_CollidingActors.size();i++)
	{
		//Creating MatrixAtoWorld
		Matrix4X4 MatrixATranslation;
		Matrix4X4 MatrixARotation;
		Matrix4X4 MatrixAtoWorld;
		MatrixATranslation.createTranslation(m_CollidingActors[i].m_Actor->getPositionx(),m_CollidingActors[i].m_Actor->getPositiony(),m_CollidingActors[i].m_Actor->getPositionz());
		MatrixARotation.createZRotation(degreesToRadians(m_CollidingActors[i].m_Actor->getRotationz()));
		MatrixAtoWorld=MatrixATranslation*MatrixARotation;

		Matrix4X4 MatrixBTranslation;
		Matrix4X4 MatrixBRotation;
		Matrix4X4 MatrixBtoWorld;
		//Creating MatrixWorldtoA
		Matrix4X4 MatrixWorldtoA;
		MatrixWorldtoA=MatrixAtoWorld.getInverse();
		//Going through CollidableActors and checking for collisions
		for(unsigned int j=0;j<m_CollidingActors.size();j++)
		{
			if(i!=j)
			{
				if((m_CollidingActors[i].m_CollisionMask & m_CollidingActors[j].m_BitMask)!=0 || (m_CollidingActors[j].m_CollisionMask & m_CollidingActors[i].m_BitMask)!=0)
				{
					if(check[j][i])
					{
						//Creating MatrixBtoA
						
						MatrixBTranslation.createTranslation(m_CollidingActors[j].m_Actor->getPositionx(),m_CollidingActors[j].m_Actor->getPositiony(),m_CollidingActors[j].m_Actor->getPositionz());
						MatrixBRotation.createZRotation(degreesToRadians(m_CollidingActors[j].m_Actor->getRotationz()));
						MatrixBtoWorld=MatrixBTranslation*MatrixBRotation;
						Matrix4X4 MatrixBtoA=MatrixWorldtoA*MatrixBtoWorld;

						//Finding velocity of B relative to A
						Vector3 BRelativeVelocity((MatrixWorldtoA*(m_CollidingActors[j].m_Actor->getVelocity()-m_CollidingActors[i].m_Actor->getVelocity())).toVector3());

						//Finding Extents of B in A
						float xExtentsBinA=abs((MatrixBtoA*Vector3(m_CollidingActors[j].m_Collider.getHalfWidth(),0.0f,0.0f)).toVector3()*Vector3(1,0,0))
										  +abs((MatrixBtoA*Vector3(0.0f,m_CollidingActors[j].m_Collider.getHalfHeight(),0.0f)).toVector3()*Vector3(1,0,0))
										+abs((MatrixBtoA*Vector3(0.0f, 0.0f ,m_CollidingActors[j].m_Collider.getHalfDepth())).toVector3()*Vector3(1, 0, 0));

						float yExtentsBinA=abs((MatrixBtoA*Vector3(m_CollidingActors[j].m_Collider.getHalfWidth(),0.0f,0.0f)).toVector3()*Vector3(0,1,0))
										  +abs((MatrixBtoA*Vector3(0.0f,m_CollidingActors[j].m_Collider.getHalfHeight(),0.0f)).toVector3()*Vector3(0,1,0))
											+abs((MatrixBtoA*Vector3(0.0f, 0.0f, m_CollidingActors[j].m_Collider.getHalfDepth())).toVector3()*Vector3(0, 1, 0));

						float zExtentsBinA = abs((MatrixBtoA*Vector3(m_CollidingActors[j].m_Collider.getHalfWidth(), 0.0f, 0.0f)).toVector3()*Vector3(0, 0, 1))
							+ abs((MatrixBtoA*Vector3(0.0f, m_CollidingActors[j].m_Collider.getHalfHeight(), 0.0f)).toVector3()*Vector3(0, 0, 1))
						+abs((MatrixBtoA*Vector3(0.0f, 0.0f, m_CollidingActors[j].m_Collider.getHalfDepth())).toVector3()*Vector3(0, 0, 1));

						//Adjusting the bounding box of A by adding the extents of B
						float tempAHalfWidth=(m_CollidingActors[i].m_Collider.getHalfWidth()+xExtentsBinA);
						float tempAHalfHeight=(m_CollidingActors[i].m_Collider.getHalfHeight()+yExtentsBinA);
						float tempAHalfDepth = (m_CollidingActors[i].m_Collider.getHalfDepth() + zExtentsBinA);
						//Finding the relative position of B in A's coordinate system
						Vector3 BRelativePosition((MatrixBtoA*Vector4(0.0f,0.0f,0.0f,1.0f)).toVector3());

						float LastEntry=1.0f;
						float FirstExit=1.0f;
						float xTimeEntry=1.0f;
						float xTimeExit=1.0f;
						float yTimeEntry=1.0f;
						float yTimeExit = 1.0f;
						float zTimeEntry = 1.0f;
						float zTimeExit = 1.0f;
						float time=0.0f;
						float FrameTime=i_FrameTime;

						//Checking for Collisions
						//IN X
						if(!isApproxZero(BRelativeVelocity.getx()))
						{
							xTimeEntry=(tempAHalfWidth-BRelativePosition.getx())/BRelativeVelocity.getx();
							xTimeExit =(-tempAHalfWidth-BRelativePosition.getx())/BRelativeVelocity.getx();
							if(xTimeEntry>xTimeExit)
							{
								float temp=xTimeEntry;  
								xTimeEntry=xTimeExit;
								xTimeExit=temp;
							}
							if(xTimeEntry> FrameTime)
							{
								check[i][j]=false;
								continue;
							}
							if(xTimeExit<0.0f)
							{
								check[i][j]=false;
								continue;
							}
							else
							{
								if(xTimeEntry<0.0f)
									LastEntry=0.0f;
								else
									LastEntry=xTimeEntry/FrameTime;
								FirstExit=xTimeExit/FrameTime;
							}
						}
						else
						{
							if(BRelativePosition.getx() > -tempAHalfWidth && BRelativePosition.getx() < tempAHalfWidth)
							{
								LastEntry=0.0f;
								FirstExit=1.0f;
								if(!isApproxZero(BRelativeVelocity.gety()))
									m_CollidingActors[i].m_Info.m_Normal=Vector3(0.0f,-1*abs(BRelativeVelocity.gety())/BRelativeVelocity.gety(),0.0f);
							}
							else
							{
								check[i][j]=false;
								continue;
							}
						}

						//IN Y
						if(!isApproxZero(BRelativeVelocity.gety()))
						{
							yTimeEntry=(tempAHalfHeight-BRelativePosition.gety())/BRelativeVelocity.gety();
							yTimeExit =(-tempAHalfHeight-BRelativePosition.gety())/BRelativeVelocity.gety(); 
							if(yTimeEntry>yTimeExit)
							{
								float temp=yTimeEntry;
								yTimeEntry=yTimeExit;
								yTimeExit=temp;
							}
							if(yTimeEntry> FrameTime)
							{
								check[i][j]=false;
								continue;
							}
							if(yTimeExit<0.0f)
							{
								check[i][j]=false;
								continue;
							}
							else
							{
								if(yTimeEntry<0.0f)
									LastEntry=LastEntry;
								else
									LastEntry=max(LastEntry,yTimeEntry/FrameTime);
								if(xTimeEntry>yTimeEntry)
								{
									if(!isApproxZero(BRelativeVelocity.getx()))
										m_CollidingActors[i].m_Info.m_Normal=Vector3(-1*abs(BRelativeVelocity.getx())/BRelativeVelocity.getx(),0.0f,0.0f);
								}
								else
								{
									if(!isApproxZero(BRelativeVelocity.getx()))
										m_CollidingActors[i].m_Info.m_Normal=Vector3(0.0f,-1*abs(BRelativeVelocity.gety())/BRelativeVelocity.gety(),0.0f);
								}
								FirstExit=min(FirstExit,yTimeExit/FrameTime);
							}
						}
						else
						{
							if(BRelativePosition.gety() > -tempAHalfHeight && BRelativePosition.gety() < tempAHalfHeight)
							{
								FirstExit=min(FirstExit,1.0f);
								if(!isApproxZero(BRelativeVelocity.getx()))
									m_CollidingActors[i].m_Info.m_Normal=Vector3(-1*abs(BRelativeVelocity.getx())/BRelativeVelocity.getx(),0.0f,0.0f);
							}
							else
							{
								check[i][j]=false;
								continue;
							}
						}
						
						//IN Z
						if (!isApproxZero(BRelativeVelocity.getz()))
						{
							zTimeEntry = (tempAHalfDepth - BRelativePosition.getz()) / BRelativeVelocity.getz();
							zTimeExit = (-tempAHalfDepth - BRelativePosition.getz()) / BRelativeVelocity.getz();
							if (zTimeEntry>zTimeExit)
							{
								float temp = zTimeEntry;
								zTimeEntry = zTimeExit;
								zTimeExit = temp;
							}
							if (zTimeEntry> FrameTime)
							{
								check[i][j] = false;
								continue;
							}
							if (zTimeExit<0.0f)
							{
								check[i][j] = false;
								continue;
							}
							else
							{
								if (zTimeEntry<0.0f)
									LastEntry = LastEntry;
								else
									LastEntry = max(LastEntry, zTimeEntry / FrameTime);
								if (zTimeEntry>zTimeEntry)
								{
									//if (!isApproxZero(BRelativeVelocity.getx()))
										//m_CollidingActors[i].m_Info.m_Normal = Vector3(-1 * abs(BRelativeVelocity.getx()) / BRelativeVelocity.getx(), 0.0f, 0.0f);
								}
								else
								{
									//if (!isApproxZero(BRelativeVelocity.getx()))
										//m_CollidingActors[i].m_Info.m_Normal = Vector3(0.0f, -1 * abs(BRelativeVelocity.gety()) / BRelativeVelocity.gety(), 0.0f);
								}
								FirstExit = min(FirstExit, zTimeExit / FrameTime);
							}
						}
						else
						{
							if (BRelativePosition.getz() > -tempAHalfDepth && BRelativePosition.getz() < tempAHalfDepth)
							{
								FirstExit = min(FirstExit, 1.0f);
								//if (!isApproxZero(BRelativeVelocity.getz()))
									//m_CollidingActors[i].m_Info.m_Normal = Vector3(-1 * abs(BRelativeVelocity.getx()) / BRelativeVelocity.getx(), 0.0f, 0.0f);
							}
							else
							{
								check[i][j] = false;
								continue;
							}
						}
						if(LastEntry<FirstExit)
						{
							m_CollidingActors[i].m_Info.m_ThisID=i;
							m_CollidingActors[i].m_Info.m_OtherID=j;
							m_CollidingActors[i].m_Info.m_Collision=true;
							m_CollidingActors[i].m_Info.m_TimeOfCollision=LastEntry;
							m_CollidingActors[i].m_Info.m_Normal=(MatrixAtoWorld*m_CollidingActors[i].m_Info.m_Normal).toVector3();
							SingleCollisions.push_back(m_CollidingActors[i]);
						}
						else
						{
							check[i][j]=false;
						}
					}
				}
			}
		}
	}
	if(SingleCollisions.size()>0)
		return ResolveCollisions(SingleCollisions,i_FrameTime); 
	else
		return i_FrameTime;
}


//Resolve the Collisions
float Engine::CollisionSystem::ResolveCollisions(std::vector<CollisionSystem> i_SingleCollisions,float i_FrameTime)
{
	std::vector<CollisionSystem> ValidatedCollisions;
	bool Colli=false;
	for(unsigned int i=0;i<i_SingleCollisions.size();i++)
	{
		for(unsigned int j=i+1;j<i_SingleCollisions.size();j++)
		{
			if(i_SingleCollisions[i].m_Info.m_ThisID==i_SingleCollisions[j].m_Info.m_OtherID && i_SingleCollisions[i].m_Info.m_OtherID==i_SingleCollisions[j].m_Info.m_ThisID)
			{
				i_SingleCollisions[i].m_Info.m_TimeOfCollision=i_SingleCollisions[j].m_Info.m_TimeOfCollision=max(i_SingleCollisions[i].m_Info.m_TimeOfCollision,i_SingleCollisions[j].m_Info.m_TimeOfCollision);
				Vector3 temp=i_SingleCollisions[i].m_Info.m_Normal;
				i_SingleCollisions[i].m_Info.m_Normal=i_SingleCollisions[j].m_Info.m_Normal;
				i_SingleCollisions[j].m_Info.m_Normal=temp;
				ValidatedCollisions.push_back(i_SingleCollisions[i]);
				ValidatedCollisions.push_back(i_SingleCollisions[j]);
				Colli=true;
			}
		}
	}
	if(!Colli)
		return i_FrameTime;

	unsigned int FirstCollision;
	float FirstCollisionTime=100.0f;

	//Find the Time of the First Collision
	for(unsigned int i=0;i< ValidatedCollisions.size();i++)
	{
		if(ValidatedCollisions[i].m_Info.m_TimeOfCollision<FirstCollisionTime)
		{
			FirstCollisionTime=ValidatedCollisions[i].m_Info.m_TimeOfCollision;
			FirstCollision=i;
		}
	}

	//Update All Actors to Time of First Collision
	for ( unsigned int i = 0; i < Engine::World::getActorList().size(); i++)								
	{																				
		Engine::World::getActorList()[i]->update(FirstCollisionTime*i_FrameTime);
	}

	//Engine::Renderer2D::displayActors();

	//Call the Collisions Callback Functions for the colliding Actors
	for(unsigned int i=0;i< ValidatedCollisions.size();i++)
	{
		if(compareFloats(ValidatedCollisions[i].m_Info.m_TimeOfCollision,FirstCollisionTime,6))
		{
			ValidatedCollisions[i].m_Info.m_CallBack(ValidatedCollisions[i].m_Actor,ValidatedCollisions[i].m_Info);
		}
	}
	checkForDeath();
	//Check for More Collisions for the updated actors
	return checkForCollisions(i_FrameTime-(FirstCollisionTime*i_FrameTime));
}


