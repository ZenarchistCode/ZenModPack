class Zen_ArtilleryBomb extends Grenade_Base
{
	void Zen_ArtilleryBomb()
	{
		SetAmmoType("Zen_ArtilleryBomb_Ammo");
		SetFuseDelay(1);
	}

	override void OnExplosionEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, float energyFactor, float explosionFactor, bool isWater, string ammoType)
	{
		super.OnExplosionEffects(source, directHit, componentIndex, surface, pos, surfNormal, energyFactor, explosionFactor, isWater, ammoType);

		if (GetGame().IsDedicatedServer() || !GetGame().IsClient())
			return;

		vector n = surfNormal.VectorToAngles() + "0 90 0";

		Particle p2 = ParticleManager.GetInstance().PlayInWorld(ParticleList.EXPLOSION_LANDMINE, pos);
		p2.SetOrientation(n);

		Particle p3 = ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_METAL_RICOCHET, pos);
		p3.SetOrientation(n);

		Particle p4 = ParticleManager.GetInstance().PlayInWorld(ParticleList.IMPACT_GRAVEL_RICOCHET, pos);
		p4.SetOrientation(n);

		float dist = vector.Distance(pos, GetGame().GetPlayer().GetPosition());

		if (dist < 50)
			GetGame().GetPlayer().GetCurrentCamera().SpawnCameraShake(Math.InverseLerp(100, 0, dist) * 2);
	}

	override void Explode(int damageType, string ammoType = "")
	{
		super.Explode(damageType, ammoType);

		if (!GetGame().IsDedicatedServer())
			return;

		SynchExplosion();
		vector offset = Vector(0, 0.1, 0);
		DamageSystem.ExplosionDamage(this, NULL, ammoType, GetPosition() + offset, damageType);
		DeleteThis();

		// Damage nearby objects defined in config
		DamageNearbyObjects(GetPosition());
	}

	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		Explode(DT_EXPLOSION, "Zen_ArtilleryBomb_Ammo");
	}

	void DeleteThis()
	{
		m_DeleteTimer = new Timer(CALL_CATEGORY_SYSTEM);
		m_DeleteTimer.Run(30, this, "DeleteSafe");
	}

	// Process object damage
	static void DamageNearbyObjects(vector position)
	{
		// Check if bomb damage is enabled
		if (GetZenArtilleryConfig().BombDamageRadius <= 0 || (GetZenArtilleryConfig().DamageObjects.Count() <= 0 && GetZenArtilleryConfig().CarMinDamage <= 0))
			return;

		// Get objects at position, check for fence/bbp/door and deal damage
		array<Object> objectsNearBomb = new array<Object>;
		GetGame().GetObjectsAtPosition(position, GetZenArtilleryConfig().BombDamageRadius, objectsNearBomb, null);
		for (int x = 0; x < objectsNearBomb.Count(); x++)
		{
			Object obj = objectsNearBomb[x];

			// Check for trees and bushes
			if (GetZenArtilleryConfig().DestroyTrees)
			{
				if (obj.IsInherited(TreeSoft) || obj.IsInherited(BushHard) || obj.IsInherited(BushSoft) || obj.IsInherited(TreeHard))
				{
					obj.SetHealth(0);
					dBodyDestroy(obj);
				}
			}

			EntityAI entity = EntityAI.Cast(obj);
			if (!entity)
				return;

			// If entities are safe under roof, check for roof
			if (GetZenArtilleryConfig().SafeUnderRoof)
			{
				if (MiscGameplayFunctions.IsUnderRoof(entity))
					return;
			}

			// Check if car damage is enabled
			if (GetZenArtilleryConfig().CarMinDamage > 0 && GetZenArtilleryConfig().CarMaxDamage > 0)
			{
				// Check if object is a car
				CarScript car = CarScript.Cast(objectsNearBomb[x]);

				// If the object is a car, damage parts
				if (car)
				{
					float damageToApply = Math.RandomFloatInclusive(GetZenArtilleryConfig().CarMinDamage, GetZenArtilleryConfig().CarMaxDamage);
					string carPart = GetZenArtilleryConfig().DamageCarParts.GetRandomElement();

					// Check if the given car part is an attachment slot
					EntityAI carAttachment = EntityAI.Cast(car.FindAttachmentBySlotName(carPart));
					if (carAttachment)
					{
						carAttachment.SetHealth(carAttachment.GetHealth() - damageToApply);
					}
					else
					{
						// Check if the given car part is an attachment classname
						carAttachment = car.GetAttachmentByConfigTypeName(carPart);
						if (carAttachment)
						{
							carAttachment.SetHealth(carAttachment.GetHealth() - damageToApply);
						}
						else
						{
							// Check if the given car part is a car damage zone (eg. "Engine")
							string carPartLower = carPart;
							carPartLower.ToLower();
							array<string> zone_names = new array<string>;
							car.GetDamageZones(zone_names);
							bool zoneFound = false;

							foreach (string zone : zone_names)
							{
								zone.ToLower();
								if (zone.Contains(carPartLower))
								{
									car.AddHealth(carPart, "", -damageToApply);
									zoneFound = true;
								}
							}

							// No damage zone was found...
							if (!zoneFound)
							{
								// Finally, cycle through all attachments and damage any that are found (eg. Wheel will damage any car type's wheel)
								for (int i = 0; i < car.GetInventory().AttachmentCount(); i++)
								{
									EntityAI attachment = car.GetInventory().GetAttachmentFromIndex(i);
									string attType = attachment.GetType();
									attType.ToLower();
									if (attType.Contains(carPartLower))
									{
										attachment.SetHealth(attachment.GetHealth() - damageToApply);
										break;
									}
								}
							}
						}
					}
				}
			}

			// If the object is a DamageObject defined in config, process damage
			if (GetZenArtilleryConfig().DamageObjects.Count() > 0)
			{
				string type = entity.GetType();
				type.ToLower();

				foreach (ArtilleryDamageObject dmg_config : GetZenArtilleryConfig().DamageObjects)
				{
					string key = dmg_config.Type;
					key.ToLower();

					if (type.Contains(key) || entity.IsKindOf(key))
					{
						entity.SetHealth(entity.GetHealth() - dmg_config.Damage);
					}
				}
			}
		}
	}
}