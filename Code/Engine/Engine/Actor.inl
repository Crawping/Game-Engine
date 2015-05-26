
//		CONSTRUCTORS

inline Engine::Actor::Actor(const char* i_pName,const char* i_pType, const Vector3 i_Position, const Vector3 i_Velocity, const Vector3 i_Acceleration,
							IActorController* i_Controller, const float i_MaxVelocity): m_pName(i_pName), m_pType(i_pType),m_Position(i_Position),
							m_Velocity(i_Velocity), m_Acceleration(i_Acceleration), m_MaxVelocity(i_MaxVelocity)
{

	m_Controllers.push_back(i_Controller);
	m_isMarkedForDeath=false;
	m_isDone = false;
	m_bIsRenderable = true;
}

inline Engine::Actor::Actor(const Engine::Actor& i_Actor)
{
	setPositionx(i_Actor.getPositionx());
	setPositionx(i_Actor.getPositiony());
	setPositionx(i_Actor.getPositionz());
	m_pName = _strdup(i_Actor.m_pName);
}


//		POSITION FUNCTIONS
inline const Engine::Vector3& Engine::Actor::getPosition() const
{
	return m_Position;
}


inline float Engine::Actor::getPositionx() const
{
	return m_Position.getx();
}
inline float Engine::Actor::getPositiony() const
{
	return m_Position.gety();
}
inline float Engine::Actor::getPositionz() const
{
	return m_Position.getz();
}

inline void Engine::Actor::setPosition(const Engine::Vector3& i_Position)
{
	m_Position = i_Position;
}

inline void Engine::Actor::setPosition(float i_x, float i_y, float i_z)
{
	m_Position.setx(i_x);
	m_Position.sety(i_y);
	m_Position.setz(i_z);

}


inline void Engine::Actor::setPositionx(const float i_x)
{
	m_Position.setx(i_x);
}
inline void Engine::Actor::setPositiony(const float i_y)
{
	m_Position.sety(i_y);
}
inline void Engine::Actor::setPositionz(const float i_z)
{
	m_Position.setz(i_z);
}


//		VELOCITY FUNCTIONS



inline const Engine::Vector3& Engine::Actor::getVelocity() const
{
	return m_Velocity;
}


inline float Engine::Actor::getVelocityx() const
{
	return m_Velocity.getx();
}
inline float Engine::Actor::getVelocityy() const
{
	return m_Velocity.gety();
}
inline float Engine::Actor::getVelocityz() const
{
	return m_Velocity.getz();
}

inline void Engine::Actor::setVelocity(const Engine::Vector3& i_Velocity)
{
	m_Velocity = i_Velocity;
}

inline void Engine::Actor::setVelocity(const float i_x, const float i_y, const float i_z)
{
	m_Velocity.setx(i_x);
	m_Velocity.sety(i_y);
	m_Velocity.setz(i_z);

}


inline void Engine::Actor::setVelocityx(const float i_x)
{
	m_Velocity.setx(i_x);
}
inline void Engine::Actor::setVelocityy(const float i_y)
{
	m_Velocity.sety(i_y);
}
inline void Engine::Actor::setVelocityz(const float i_z)
{
	m_Velocity.setz(i_z);
}


//		ACCELERATION FUNCTIONS
inline const Engine::Vector3& Engine::Actor::getAcceleration() const
{
	return m_Acceleration;
}


inline float Engine::Actor::getAccelerationx() const
{
	return m_Acceleration.getx();
}
inline float Engine::Actor::getAccelerationy() const
{
	return m_Acceleration.gety();
}
inline float Engine::Actor::getAccelerationz() const
{
	return m_Acceleration.getz();
}

inline void Engine::Actor::setAcceleration(const Engine::Vector3& i_Acceleration)
{
	m_Acceleration = i_Acceleration;
}

inline void Engine::Actor::setAcceleration(const float i_x, const float i_y, const float i_z)
{
	m_Acceleration.setx(i_x);
	m_Acceleration.sety(i_y);
	m_Acceleration.setz(i_z);

}


inline void Engine::Actor::setAccelerationx(const float i_x)
{
	m_Acceleration.setx(i_x);
}
inline void Engine::Actor::setAccelerationy(const float i_y)
{
	m_Acceleration.sety(i_y);
}
inline void Engine::Actor::setAccelerationz(const float i_z)
{
	m_Acceleration.setz(i_z);
}

//		ROTATION FUNCTIONS
inline const Engine::Vector3& Engine::Actor::getRotation() const
{
	return m_Rotation;
}


inline float Engine::Actor::getRotationx() const
{
	return m_Rotation.getx();
}
inline float Engine::Actor::getRotationy() const
{
	return m_Rotation.gety();
}
inline float Engine::Actor::getRotationz() const
{
	return m_Rotation.getz();
}

inline void Engine::Actor::setRotation(const Engine::Vector3& i_Rotation)
{
	m_Rotation=i_Rotation;
}

inline void Engine::Actor::setRotation(float i_x, float i_y, float i_z)
{
	m_Rotation.setx(i_x);
	m_Rotation.sety(i_y);
	m_Rotation.setz(i_z);

}


inline void Engine::Actor::setRotationx(const float i_x)
{
	m_Rotation.setx(i_x);
}
inline void Engine::Actor::setRotationy(const float i_y)
{
	m_Rotation.sety(i_y);
}
inline void Engine::Actor::setRotationz(const float i_z)
{
	m_Rotation.setz(i_z);
}


//		SCALE FUNCTIONS
inline const Engine::Vector3& Engine::Actor::getScale() const
{
	return m_Scale;
}


inline float Engine::Actor::getScalex() const
{
	return m_Scale.getx();
}
inline float Engine::Actor::getScaley() const
{
	return m_Scale.gety();
}
inline float Engine::Actor::getScalez() const
{
	return m_Scale.getz();
}

inline void Engine::Actor::setScale(const Engine::Vector3& i_Scale)
{
	m_Scale = i_Scale;
}

inline void Engine::Actor::setScale(const float i_x,const float i_y,const float i_z)
{
	m_Scale.setx(i_x);
	m_Scale.sety(i_y);
	m_Scale.setz(i_z);

}


inline void Engine::Actor::setScalex(const float i_x)
{
	m_Scale.setx(i_x);
}
inline void Engine::Actor::setScaley(const float i_y)
{
	m_Scale.sety(i_y);
}
inline void Engine::Actor::setScalez(const float i_z)
{
	m_Scale.setz(i_z);
}


//---------------x-x-x-----------------


inline void Engine::Actor::setMaxVelocity(const float i_Velocity)
{
	m_MaxVelocity = i_Velocity;
}

inline float Engine::Actor::getMaxVelocity()
{
	return m_MaxVelocity;
}

inline void Engine::Actor::addController(IActorController* i_IActorController)
{
	m_Controllers.push_back(i_IActorController);
}

inline void Engine::Actor::removeController()
{
	m_Controllers.clear();
}
inline bool Engine::Actor::getisDone() const
{
	return m_isDone;
}
inline void Engine::Actor::setisDone(const bool i_value)
{
	m_isDone = i_value;
}

inline void Engine::Actor::setMarkedForDeath(const bool i_value)
{
	m_isMarkedForDeath = i_value;
}
inline bool Engine::Actor::getMarkedForDeath() const
{
	return m_isMarkedForDeath;
}

inline void Engine::Actor::releasePointer()
{
	/*for (unsigned int i = 0; i < m_Controllers.size();i++)
		m_Controllers[i].ReleasePointer();*/
}

inline const char* Engine::Actor::getType()
{
	return m_pType;
} 

inline Engine::Vector3 Engine::Actor::getForward() const
{

	D3DXMATRIX MatTemp;  // Temp matrix for rotations.
	D3DXMATRIX MatRot;   // Final rotation matrix, applied to 
	D3DXMATRIX MatWorld;

	// Using the left-to-right order of matrix concatenation,
	// apply the translation to the object's world position
	// before applying the rotations

	D3DXMatrixIdentity(&MatRot);
	D3DXMatrixRotationX(&MatTemp, m_Rotation.getx());         // Pitch
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXMatrixRotationY(&MatTemp, m_Rotation.gety());           // Yaw
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXMatrixRotationZ(&MatTemp, m_Rotation.getz());          // Roll
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXVECTOR4 temp;
	D3DXVECTOR3 forward = Vector3(0.0f, 0.0f, 1.0f).getD3DVector3();
	D3DXVec3Transform(&temp, &forward, &MatRot);
	return Vector3(temp.x, temp.y, temp.z).normalized();
}

inline Engine::Vector3 Engine::Actor::getRight() const
{

	D3DXMATRIX MatTemp;  // Temp matrix for rotations.
	D3DXMATRIX MatRot;   // Final rotation matrix, applied to 
	D3DXMATRIX MatWorld;

	// Using the left-to-right order of matrix concatenation,
	// apply the translation to the object's world position
	// before applying the rotations
	D3DXMatrixIdentity(&MatRot);

	D3DXMatrixRotationX(&MatTemp, m_Rotation.getx());         // Pitch
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXMatrixRotationY(&MatTemp, m_Rotation.gety());           // Yaw
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXMatrixRotationZ(&MatTemp, m_Rotation.getz());          // Roll
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXVECTOR4 temp;
	D3DXVECTOR3 forward = Vector3(1.0f, 0.0f, 0.0f).getD3DVector3();
	D3DXVec3Transform(&temp, &forward, &MatRot);
	return Vector3(temp.x, temp.y, temp.z).normalized();
}


inline void Engine::Actor::setForward(Engine::Vector3 i_vector) 
{
	m_Forward = i_vector;
}