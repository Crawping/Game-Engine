#ifndef ACTOR_H
#define ACTOR_H
#include "Vector3.h"
#include "IActorController.h"
#include "MemoryPool.h"
#include "SharedPointer.h"
#include "Mesh.h"
#include "Material.h"


namespace Engine
{
	class Actor															
	{

		Vector3 m_Position;
		Vector3 m_Velocity;
		Vector3 m_Acceleration;
		Vector3 m_Rotation;
		Vector3 m_Forward;
		Vector3 m_Scale;
		float m_MaxVelocity;
		bool m_isDone;
		bool m_isMarkedForDeath;
		Material* m_Material;
		Mesh* m_Mesh;
		const char* m_pName;
		const char* m_pType;
		bool m_bIsRenderable;
		std::vector<IActorController*> m_Controllers; 
	public:
		static MemoryPool* m_pActorPool;
		inline Actor(void){ m_bIsRenderable = true; };															//Default Constructor
		inline Actor(const Actor&);														//Copy Constructor
		inline Actor(const char*, const char*, const Vector3, const Vector3, const Vector3, IActorController*, const float);
		~Actor(){ if (m_pName != nullptr) delete m_pName; };

		void* operator new(size_t i_size);												//Overloaded new using memory pool
		void operator delete(void* i_ptr);												//Overloaded delete corresponding to the new
		void update(const float);															//Updates the state of the Actor
		void randomizePosition();														//Assign random values to Position Vector

		//-----------POSITION----------
		inline const Vector3& getPosition() const;
		inline float getPositionx() const;												//Get Functions													
		inline float getPositiony() const;
		inline float getPositionz() const;
		
		inline void setPosition(const Vector3&);
		inline void setPosition(const float, const float, const float);					//Set Functions
		inline void setPositionx(const float);											
		inline void setPositiony(const float);
		inline void setPositionz(const float);


		//-----------VELOCITY-----------
		inline const Vector3& getVelocity() const;
		inline float getVelocityx() const;												//Get Functions													
		inline float getVelocityy() const;
		inline float getVelocityz() const;

		inline void setVelocity(const Vector3&);
		inline void setVelocity(const float, const float, const float);					//Set Functions
		inline void setVelocityx(const float);											
		inline void setVelocityy(const float);
		inline void setVelocityz(const float);


		//----------ACCELERATION-----------
		inline const Vector3& getAcceleration() const;
		inline float getAccelerationx() const;												//Get Functions													
		inline float getAccelerationy() const;
		inline float getAccelerationz() const;
		
		inline void setAcceleration(const Vector3&);
		inline void setAcceleration(const float, const float, const float);					//Set Functions
		inline void setAccelerationx(const float);											
		inline void setAccelerationy(const float);
		inline void setAccelerationz(const float);

		//----------FORWARD-----------
		inline Vector3 getForward() const;
		inline Vector3 getRight() const;

		inline void setForward(Vector3 i_vector);


		//----------Rotation-----------
		inline const Vector3& getRotation() const;
		inline float getRotationx() const;												//Get Functions													
		inline float getRotationy() const;
		inline float getRotationz() const;
		
		inline void setRotation(const Vector3&);
		inline void setRotation(const float, const float, const float);					//Set Functions
		inline void setRotationx(const float);											
		inline void setRotationy(const float);
		inline void setRotationz(const float);

		//----------Scale-----------
		inline const Vector3& getScale() const;
		inline float getScalex() const;												//Get Functions													
		inline float getScaley() const;
		inline float getScalez() const;

		inline void setScale(const Vector3&);
		inline void setScale(const float, const float, const float);					//Set Functions
		inline void setScalex(const float);
		inline void setScaley(const float);
		inline void setScalez(const float);
		

		//---------Material------------
		inline void SetMaterial(Material* i_Material){ m_Material = i_Material; };
		inline  Material* GetMaterial(){ return m_Material; };


		//-----------Mesh-------------
		inline void SetMesh(Mesh* i_Mesh){ m_Mesh = i_Mesh; };
		inline  Mesh* GetMesh(){ return m_Mesh; };

		//----------Get Position as Matrix Transform-----
		D3DXMATRIX GetTransform();

		//-----------Set Name-------------
		void SetName(const char* i_name);
		const char* GetName(){ return m_pName; };

		inline const char* getType();
		inline float getMaxVelocity();
		inline void setMaxVelocity(const float);
		inline bool getisDone() const;														
		virtual inline void addController(IActorController*);								//Set Controller
		virtual inline void removeController();
		inline void setisDone(const bool);													//Updates when Controller has finished Updating
		inline void setMarkedForDeath(const bool);
		inline bool getMarkedForDeath() const;
		inline bool GetIsRenderable(){ return m_bIsRenderable; };
		inline void SetIsRenderable(bool i_value){ m_bIsRenderable=i_value; };

		inline void releasePointer();
	};
}
#include "Actor.inl"
#endif