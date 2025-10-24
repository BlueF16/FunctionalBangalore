	// SCR_BangaloreDuplicateAction.c
	class SCR_BangaloreDuplicateAction : ScriptedUserAction
	{
		[Attribute(defvalue: "1.5", desc: "Offset forward distance in meters for the new Bangalore")]
	    protected float m_fOffset;
	
	
		[Attribute("",UIWidgets.ResourceNamePicker)]
		protected ResourceName spawn_object;
	
	
		[Attribute("",UIWidgets.ResourceNamePicker)]
		protected ResourceName inventory_object;
	

		ref array<IEntity> SPAWNEDBANGS = {};
	
	
	
		array<IEntity> GetSpawnBangs()
	{
		return SPAWNEDBANGS;
	}
			
	
	
	
		bool HasItem(IEntity pUserEntity, ResourceName inventory_object)
    	{
        	if (!pUserEntity || inventory_object.IsEmpty())
            	return false;
        
        // Get the inventory storage manager from the player
        InventoryStorageManagerComponent inventoryManager = InventoryStorageManagerComponent.Cast(pUserEntity.FindComponent(InventoryStorageManagerComponent));
        
        	if (!inventoryManager)
        	{
            	Print("No inventory manager found on player", LogLevel.WARNING);
            	return false;
        	}
        
       		// Get all items from the inventory
        	array<IEntity> allItems = {};
        	inventoryManager.GetItems(allItems);
        
        	// Check each item to see if its prefab matches what we're looking for
        	foreach (IEntity item : allItems)
        	{
            	if (!item)
               	 continue;
                
            EntityPrefabData prefabData = item.GetPrefabData();
            if (!prefabData)
                continue;
			
	
                
            // Compare the item's prefab resource name with the one we're looking for
            if (prefabData.GetPrefabName() == inventory_object)
            {
                Print("Item found in inventory: " + inventory_object, LogLevel.NORMAL);
                return true;
            }
        }
        
        Print("Item NOT found in inventory: " + inventory_object, LogLevel.WARNING);
        return false;
    	}
	
	
	    override bool CanBePerformedScript(IEntity user)
    	{
        // Check if the user has the required item before allowing the action
        if (!HasItem(user, inventory_object))
        {
            return false;
        }
		
		if (SPAWNEDBANGS.Count()>4)
		{
			return false;
		}
				                return true;
	    }
		
			
			override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
		{	
		
			

			
			vector mat[4];
			pOwnerEntity.GetTransform(mat);
			vector orient=pOwnerEntity.GetYawPitchRoll();
			float x = mat[3][0];
			float z = mat[3][2];
			float angley = orient[0]+90;
			x = x + Math.Sin(angley * Math.DEG2RAD) * m_fOffset * (SPAWNEDBANGS.Count()+1);
			z = z + Math.Cos(angley * Math.DEG2RAD) * m_fOffset* (SPAWNEDBANGS.Count()+1);
			
			
			
			float y = SCR_TerrainHelper.GetTerrainY({x,0,z},GetGame().GetWorld());
			
			vector newpos = {x,y,z};
			mat[3]=newpos;
			
			EntitySpawnParams PARAM_SPAWN = new EntitySpawnParams();
			PARAM_SPAWN.Transform=mat;
			
			
			SPAWNEDBANGS.Insert(GetGame().SpawnEntityPrefab(Resource.Load(spawn_object),GetGame().GetWorld(),PARAM_SPAWN));
			RemoveOneMatchingItemOnServer(pUserEntity, inventory_object);
		}
	
	
	
	protected void RemoveOneMatchingItemOnServer(IEntity pUserEntity, ResourceName wantPrefab)
	{
		if (!Replication.IsServer()) return;
		if (!pUserEntity || wantPrefab.IsEmpty()) return;

		InventoryStorageManagerComponent inv = InventoryStorageManagerComponent.Cast(
			pUserEntity.FindComponent(InventoryStorageManagerComponent)
		);
		if (!inv) return;

		// (Optional but recommended) sanity check: only process if request is valid proximity-wise
		// if (!inv.ValidateStorageRequest(pUserEntity)) return; // uncomment if you’re validating clients

		array<IEntity> allItems = {};
		inv.GetItems(allItems);

		foreach (IEntity item : allItems)
		{
			if (!item) continue;
			EntityPrefabData pd = item.GetPrefabData();
			if (!pd) continue;

			if (pd.GetPrefabName() == wantPrefab)
			{
				// Delete exactly one
				inv.TryDeleteItem(item); // server authoritative deletion; replicates to clients
				return;
			}
		
		}
	}
	
	}
