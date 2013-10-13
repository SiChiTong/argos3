/**
 * @file <argos3/plugins/simulator/physx_engines/physx/physx_engine.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_ENGINE_H
#define PHYSX_ENGINE_H

namespace argos {
   class CPhysXEngine;
   class CPhysXModel;
}

#include <argos3/core/utility/math/quaternion.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/simulator/physics_engine/physics_engine.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx-dist/include/PxPhysicsAPI.h>

namespace argos {

   /**
    * The PhysX engine.
    * <p>
    * This class implements the CPhysicsEngine interface using the NVIDIA PhysX
    * physics engine library.
    * </p>
    * <p>
    * The axes of the space in PhysX are not directed as in the ARGoS space. In the
    * latter, the XY plane is the ground and Z indicates the up direction (right-hand rule);
    * the camera is pointed along the X axis, the base of the screen is the Y axis, and the
    * left-hand vertical side of the screen corresponds to the Z axis. Conversely, in PhysX
    * the camera is oriented along the Z axis, with the X axis corresponding to the lower side
    * of the screen and the Y axis corresponding to the left-hand vertical side (left-hand rule).
    * To convert positions and orientations between the ARGoS space and the PhysX space, you
    * must use the functions CVector3ToPxVec3(), PxVec3ToCVector3(), CQuaternionToPxQuat(),
    * PxQuatToCQuaternion().
    * </p>
    */
   class CPhysXEngine : public CPhysicsEngine {

   public:

      CPhysXEngine();
      virtual ~CPhysXEngine();

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual void Update();

      virtual bool IsPointContained(const CVector3& c_point);

      virtual UInt32 GetNumPhysicsEngineEntities();

      virtual void AddEntity(CEntity& c_entity);

      virtual void RemoveEntity(CEntity& c_entity);

      virtual bool IsEntityTransferNeeded() const;

      virtual void TransferEntities();

      virtual CEmbodiedEntity* CheckIntersectionWithRay(Real& f_t_on_ray,
                                                        const CRay3& c_ray) const;

      void AddPhysicsModel(const std::string& str_id,
                           CPhysXModel& c_model);
      void RemovePhysicsModel(const std::string& str_id);

      /**
       * Returns the PhysX physics.
       * @return The PhysX physics.
       */
      inline physx::PxPhysics& GetPhysics() {
         return *m_pcPhysics;
      }

      /**
       * Returns the PhysX physics as a const reference.
       * @return The PhysX physics as a const reference.
       */
      inline const physx::PxPhysics& GetPhysics() const {
         return *m_pcPhysics;
      }

      /**
       * Returns the cooking subsystem.
       * The cooking subsystem is used to create convex meshes.
       * @return The cooking subsystem.
       */
      inline physx::PxCooking& GetCooking() {
         return *m_pcCooking;
      }

      /**
       * Returns the cooking subsystem as a const reference.
       * The cooking subsystem is used to create convex meshes.
       * @return The cooking subsystem as a const reference.
       */
      inline const physx::PxCooking& GetCooking() const {
         return *m_pcCooking;
      }

      /**
       * Returns the PhysX scene.
       * @return The PhysX scene.
       */
      inline physx::PxScene& GetScene() {
         return *m_pcScene;
      }

      /**
       * Returns the PhysX scene as a const reference.
       * @return The PhysX scene as a const reference.
       */
      inline const physx::PxScene& GetScene() const {
         return *m_pcScene;
      }

      /**
       * Returns the default material for the models.
       * Static friction = 0.7, dynamic friction = 0.5, restitution coefficient = 0.1
       * @return The default material for the models.
       */
      inline physx::PxMaterial& GetDefaultMaterial() {
         return *m_pcDefaultMaterial;
      }

   private:

      class CPhysXEngineAllocatorCallback : public physx::PxAllocatorCallback {
      public:
         virtual void* allocate(size_t size, const char*, const char*, int);
         virtual void	deallocate(void* ptr);
      };

      class CPhysXEngineErrorCallback : public physx::PxErrorCallback {
      public:
         CPhysXEngineErrorCallback(CPhysXEngine& c_engine);
         virtual ~CPhysXEngineErrorCallback() {}
         virtual void reportError(physx::PxErrorCode::Enum e_code,
                                  const char* pch_message,
                                  const char* pch_file,
                                  int n_line);
      private:
         CPhysXEngine& m_cEngine;
      };

   private:

      /** List of physics models */
      std::map<std::string, CPhysXModel*> m_tPhysicsModels;

      /** The PhysX memory allocator */
      CPhysXEngineAllocatorCallback m_cAllocatorCallback;
      /** The PhysX error callback */
      CPhysXEngineErrorCallback m_cErrorCallback;

      /** The PhysX foundation class */
      physx::PxFoundation* m_pcFoundation;
      /** The PhysX physics */
      physx::PxPhysics* m_pcPhysics;
      /** The cooking subsystem for convex meshes */
      physx::PxCooking* m_pcCooking;
      /** The PhysX CPU dispatcher */
      physx::pxtask::CpuDispatcher* m_pcCPUDispatcher;
      /** The PhysX scene descriptor */
      physx::PxSceneDesc* m_pcSceneDesc;
      /** The PhysX scene */
      physx::PxScene* m_pcScene;

      /** The default model material */
      physx::PxMaterial* m_pcDefaultMaterial;
      /** The ground plane */
      physx::PxRigidStatic* m_pcGround;

   };

   /****************************************/
   /****************************************/

   /**
    * Converts a CVector3 into a PxVec3.
    * The CVector3 corresponds to the position of an object in the ARGoS space.
    * The PxVec corresponds to the position of an object in the PhysX space.
    * @param c_vector3 The position in the ARGoS space.
    * @param c_pxvec3 The position in the PhysX space.
    * @see PxVec3ToCVector3
    * @see CQuaternionToPxQuat
    * @see PxQuatToCQuaternion
    */
   inline void CVector3ToPxVec3(const CVector3& c_vector3,
                                physx::PxVec3& c_pxvec3) {
      c_pxvec3.x = -c_vector3.GetY();
      c_pxvec3.y = c_vector3.GetZ();
      c_pxvec3.z = c_vector3.GetX();
   }

   /**
    * Converts a PxVec3 into a CVector3.
    * The PxVec corresponds to the position of an object in the PhysX space.
    * The CVector3 corresponds to the position of an object in the ARGoS space.
    * @param c_pxvec3 The position in the PhysX space.
    * @param c_vector3 The position in the ARGoS space.
    * @see CVector3ToPxVec3
    * @see CQuaternionToPxQuat
    * @see PxQuatToCQuaternion
    */
   inline void PxVec3ToCVector3(const physx::PxVec3& c_pxvec3,
                                CVector3& c_vector3) {
      c_vector3.Set(c_pxvec3.z,
                    -c_pxvec3.x,
                    c_pxvec3.y);
   }

   /**
    * Converts a CQuaternion into a PxQuat.
    * The CQuaternion corresponds to the orientation of an object in the ARGoS space.
    * The PxQuat corresponds to the orientation of an object in the PhysX space.
    * @param c_quaternion The orientation in the ARGoS space.
    * @param c_pxquat The orientation in the PhysX space.
    * @see CVector3ToPxVec3
    * @see PxVec3ToCVector3
    * @see PxQuatToCQuaternion
    */
   inline void CQuaternionToPxQuat(const CQuaternion& c_quaternion,
                                   physx::PxQuat& c_pxquat) {
      c_pxquat.w = c_quaternion.GetW();
      c_pxquat.x = c_quaternion.GetY();
      c_pxquat.y = -c_quaternion.GetZ();
      c_pxquat.z = -c_quaternion.GetX();
   }

   /**
    * Converts a PxQuat into a CQuaternion.
    * The PxQuat corresponds to the orientation of an object in the PhysX space.
    * The CQuaternion corresponds to the orientation of an object in the ARGoS space.
    * @param c_pxquat The orientation in the PhysX space.
    * @param c_quaternion The orientation in the ARGoS space.
    * @see CVector3ToPxVec3
    * @see PxVec3ToCVector3
    * @see CQuaternionToPxQuat
    */
   inline void PxQuatToCQuaternion(const physx::PxQuat& c_pxquat,
                                   CQuaternion& c_quaternion) {
      c_quaternion.SetW(c_pxquat.w);
      c_quaternion.SetX(-c_pxquat.z);
      c_quaternion.SetY(c_pxquat.x);
      c_quaternion.SetZ(-c_pxquat.y);
   }

   /****************************************/
   /****************************************/

   template <typename ACTION>
   class CPhysXOperation : public CEntityOperation<ACTION, CPhysXEngine, void> {
   public:
      virtual ~CPhysXOperation() {}
   };

   class CPhysXOperationAddEntity : public CPhysXOperation<CPhysXOperationAddEntity> {
   public:
      virtual ~CPhysXOperationAddEntity() {}
   };

   class CPhysXOperationRemoveEntity : public CPhysXOperation<CPhysXOperationRemoveEntity> {
   public:
      virtual ~CPhysXOperationRemoveEntity() {}
   };

#define REGISTER_PHYSX_OPERATION(ACTION, OPERATION, ENTITY)             \
   REGISTER_ENTITY_OPERATION(ACTION, CPhysXEngine, OPERATION, void, ENTITY);

#define REGISTER_STANDARD_PHYSX_OPERATION_ADD_ENTITY(SPACE_ENTITY, DYN2D_MODEL) \
   class CPhysXOperationAdd ## SPACE_ENTITY : public CPhysXOperationAddEntity { \
   public:                                                              \
   CPhysXOperationAdd ## SPACE_ENTITY() {}                              \
   virtual ~CPhysXOperationAdd ## SPACE_ENTITY() {}                     \
   void ApplyTo(CPhysXEngine& c_engine,                                 \
                SPACE_ENTITY& c_entity) {                               \
      DYN2D_MODEL* pcPhysModel = new DYN2D_MODEL(c_engine,              \
                                                 c_entity);             \
      c_engine.AddPhysicsModel(c_entity.GetId(),                        \
                               *pcPhysModel);                           \
      c_entity.                                                         \
         GetComponent<CEmbodiedEntity>("body").                         \
         AddPhysicsModel(c_engine.GetId(), *pcPhysModel);               \
   }                                                                    \
   };                                                                   \
   REGISTER_PHYSX_OPERATION(CPhysXOperationAddEntity,                   \
                            CPhysXOperationAdd ## SPACE_ENTITY,         \
                            SPACE_ENTITY);
   
#define REGISTER_STANDARD_PHYSX_OPERATION_REMOVE_ENTITY(SPACE_ENTITY)   \
   class CPhysXOperationRemove ## SPACE_ENTITY : public CPhysXOperationRemoveEntity { \
public:                                                                 \
CPhysXOperationRemove ## SPACE_ENTITY() {}                              \
virtual ~CPhysXOperationRemove ## SPACE_ENTITY() {}                     \
void ApplyTo(CPhysXEngine& c_engine,                                    \
      SPACE_ENTITY& c_entity) {                                         \
   c_engine.RemovePhysicsModel(c_entity.GetId());                       \
   c_entity.                                                            \
   GetComponent<CEmbodiedEntity>("body").                               \
   RemovePhysicsModel(c_engine.GetId());                                \
}                                                                       \
};                                                                      \
   REGISTER_PHYSX_OPERATION(CPhysXOperationRemoveEntity,                \
      CPhysXOperationRemove ## SPACE_ENTITY,                            \
      SPACE_ENTITY);
   
#define REGISTER_STANDARD_PHYSX_OPERATIONS_ON_ENTITY(SPACE_ENTITY, DYN2D_ENTITY) \
   REGISTER_STANDARD_PHYSX_OPERATION_ADD_ENTITY(SPACE_ENTITY, DYN2D_ENTITY) \
   REGISTER_STANDARD_PHYSX_OPERATION_REMOVE_ENTITY(SPACE_ENTITY)

/****************************************/
/****************************************/

}

#endif
