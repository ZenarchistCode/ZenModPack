You have two options with these files: either merge them all into the vanilla types.xml and cfgspawntabletypes.xml files carefully (use https://www.xmlvalidation.com/ to ensure nothing is broken afterwards)

OR ideally, you should split these files into their own types.xml entries in your economy config separate from vanilla.

Here are the steps:

1. Create a new "Zenarcist" folder in "/mpmissions/map.name/db/Zenarchist"

2. Copy & paste all the files included inside this folder into that folder.

3. Open the file "/mpmissions/map.name/cfgeconomycore.xml"

4. Add this new entry at the bottom of the cfgeconomycore.xml file BEFORE the </economycore> tag:

	<ce folder="db/Zenarchist">
		<file name="ZenCustomGear.xml" type="types" />
		<file name="ZenMusic.xml" type="types" />
		<file name="ZenTreasure.xml" type="types" />
		<file name="cfgspawnabletypes.xml" type="spawnabletypes" />
	</ce>
	
5. These types.xml files have nominals set to zero for ALL items, so you will need to go in and modify the spawn quantites to suit your server and preferences (and disable any items you don't want spawning). If you're new to DayZ modding check out the vanilla XML files for inspiration and use https://www.xmlvalidation.com/ to validate your changes and ensure you do not break your server's loot config, and the main parameters you need to tweak are:

	<nominal>#</nominal>		- The target number of spawns
	<lifetime>#</lifetime> 		- The persistence in seconds of the item
	<min>#</min>				- The minimum items in world before spawning begins again to target nominal value

6. NOTE: If you DO NOT want to use my music and/or treasure mods, don't include them in the above list.



BONUS INFO: You can also add your own custom events like this too if you want to keep them separate from vanilla (not necessary for my mod as I don't have any events, just something cool to know if you weren't aware):

	<file name="events.xml" type="events" />