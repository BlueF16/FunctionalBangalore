	// SCR_BangaloreDuplicateAction.c
	class SCR_BangaloreDuplicateAction : ScriptedUserAction
	{
		[Attribute(defvalue: "1.5", desc: "Offset forward distance in meters for the new Bangalore")]
	    protected float m_fOffset;
	
	
		[Attribute("",UIWidgets.ResourceNamePicker)]
		protected ResourceName spawn_object;
	

		ref array<IEntity> SPAWNEDBANGS = {};
			
	
	
	
		bool HasItem(IEntity pUserEntity, ResourceName spawn_object)
    	{
        	if (!pUserEntity || spawn_object.IsEmpty())
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
			
			Print(prefabData.GetPrefabName());
                
            // Compare the item's prefab resource name with the one we're looking for
            if (prefabData.GetPrefabName() == spawn_object)
            {
                Print("Item found in inventory: " + spawn_object, LogLevel.NORMAL);
                return true;
            }
        }
        
        Print("Item NOT found in inventory: " + spawn_object, LogLevel.WARNING);
        return false;
    	}
	
	
	    override bool CanBePerformedScript(IEntity user)
    	{
        // Check if the user has the required item before allowing the action
        if (!HasItem(user, spawn_object))
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
		}
	
	}
