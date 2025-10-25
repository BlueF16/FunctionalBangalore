//------------------------------------------------------------------------------------------------
//! modded version for to be used with the inventory 2.0 
class SCR_PickUpItemActionBangalore : SCR_InventoryAction
{
	#ifndef DISABLE_INVENTORY
	//------------------------------------------------------------------------------------------------
	[Attribute("",UIWidgets.ResourceNamePicker)]
		protected ResourceName inventory_object;
	
	
	
	
	
	override protected void PerformActionInternal(SCR_InventoryStorageManagerComponent manager, IEntity pOwnerEntity, IEntity pUserEntity)
	{
		
		ActionsManagerComponent am = GetActionsManager();     // inherited from BaseUserAction

		if (!am)
		{
			Print("PickupAction: no ActionsManagerComponent.");
			return;
		}

		// ② Ask the manager for all actions on this owner
		array<BaseUserAction> actions();
		am.GetActionsList(actions);                           // list of all actions on this entity

		// ③ Find our sibling: SCR_BangaloreDuplicateAction
		SCR_BangaloreDuplicateAction dupAction;
		foreach (BaseUserAction a : actions)
		{
			dupAction = SCR_BangaloreDuplicateAction.Cast(a);
			if (dupAction) break;
		}

		if (!dupAction)
		{
			Print("PickupAction: sibling SCR_BangaloreDuplicateAction not found.");
			return;
		}

		// ④ Access the shared array
		array<IEntity> spawned = dupAction.GetSpawnBangs();
		PrintFormat("PickupAction: I see %1 spawned bangs.", spawned.Count());
		
		
		auto pInventoryManager = SCR_InventoryStorageManagerComponent.Cast( pUserEntity.FindComponent( SCR_InventoryStorageManagerComponent ) );
		bool isFull = false;
		
		IEntity item;
		
		for (int i=spawned.Count()-1, count = spawned.Count(); i >= 0; --i) // optimised
			
		{
			
			manager.TrySpawnPrefabToStorage(inventory_object);
			SCR_EntityHelper.DeleteEntityAndChildren(spawned[i]);
			spawned.Remove(i);
			
						if ( !pInventoryManager.CanInsertItem( m_Item.GetOwner(), EStoragePurpose.PURPOSE_DEPOSIT ) )
					{
							isFull = true;
							break;
					}
			}
		
		dupAction.SPAWNEDBANGS = spawned;
				
		
		
		
		
		
		

		if (!isFull){
		manager.InsertItem( pOwnerEntity );
		}
	}
	
	
	//------------------------------------------------------------------------------------------------
	override bool CanBePerformedScript(IEntity user)
 	{
		if ( !super.CanBePerformedScript( user ) )
			return false;
		
		auto pInventoryManager = SCR_InventoryStorageManagerComponent.Cast( user.FindComponent( SCR_InventoryStorageManagerComponent ) );
		if ( !pInventoryManager )
			return false;
		
		pInventoryManager.SetReturnCodeDefault();
		
		if ( !pInventoryManager.IsAnimationReady() || !pInventoryManager.CanInsertItem( m_Item.GetOwner(), EStoragePurpose.PURPOSE_DEPOSIT ) )
		{
			SetCannotPerformReason( GetReason( pInventoryManager ) );
			return false;
		}
		
		return true;
 	}
	#endif
};