// SCR_BangaloreDuplicateAction.c
class SCR_BangaloreDuplicateAction : ScriptedUserAction
{
	[Attribute(defvalue: "1.5", desc: "Offset forward distance in meters for the new Bangalore")]
    protected float m_fOffset;
	[Attribute("",UIWidgets.ResourceNamePicker)]
	protected ResourceName spawn_object
	ref array<IEntity> SPAWNEDBANGS = {};
	
	
	
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

		
		Print(mat);
		
		EntitySpawnParams PARAM_SPAWN = new EntitySpawnParams();
		PARAM_SPAWN.Transform=mat;
		
		
		
		SPAWNEDBANGS.Insert(GetGame().SpawnEntityPrefab(Resource.Load(spawn_object),GetGame().GetWorld(),PARAM_SPAWN));
		
				
		
	
	}

}
