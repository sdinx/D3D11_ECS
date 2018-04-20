//----------------------------------------------------------------------------------
// file : EntityId.h
// desc : This unique id for class Entity
//----------------------------------------------------------------------------------

#ifndef  _INCLUDED_D3D11_UTILITY_ENTITY_ID_
#define  _INCLUDED_D3D11_UTILITY_ENTITY_ID_

namespace  D3D11Utility
{

		struct  EntityId
		{
				unsigned  entityId;
				inline  void  operator=( unsigned  id ) {
						entityId = id;
				}
				inline  bool  operator==( EntityId  id )const {
						return  ( entityId == id.entityId ) ? true : false;
				}
				inline  bool  operator!=( EntityId  id )const {
						return  ( entityId != id.entityId ) ? true : false;
				}
				inline  bool  operator<( EntityId  id )const {
						return  ( entityId < id.entityId ) ? true : false;
				}
				inline  bool  operator>( EntityId  id )const {
						return  ( entityId > id.entityId ) ? true : false;
				}
				inline  bool  operator<=( EntityId  id )const {
						return  ( entityId <= id.entityId ) ? true : false;
				}
				inline  bool  operator>=( EntityId  id )const {
						return  ( entityId >= id.entityId ) ? true : false;
				}
		};

}

#endif // ! _INCLUDED_D3D11_UTILITY_ENTITY_ID_