// This class creates the default ZenIceLakes.c file to make the mod more user-friendly and require minimal install steps
class ZenIcePlanesDefault
{
	private const static string zenModFolder = "$profile:\\Zenarchist\\";
	private const static string zenConfigName = "ZenIceLakes.c";

	// This is a little janky, but because we're #including the ZenIceLakes.c server-side we have an issue:
	// we need to create the default config file in 3_game so that by the time 5_mission compiles we have the default file ready.
	// But... we can't access the worldName until 5_mission loads, so I can't check what map name we're on until later.
	// BUT... the server won't compile if ZenIceLakes.c is missing. So my janky solution is to create a default empty file
	// and then when 5_mission loads, we can check the worldName - and if we only have 3 lines of code in the file then
	// we know that's my default config and it needs to be overwritten with the map config. Otherwise, 3+ lines = don't create default cfg.
	// I know - there's at least one person out there reading this who thinks I'm an idiot and knows of a better way - feel free to fix it yourself.
	static void CheckDefaultCFile(string worldName = "")
	{
		string file_path = zenModFolder + zenConfigName;

		if (!FileExist(file_path) || worldName != "")
		{
			// If file exists and world name is not blank, check that we are working with a blank cfg file before overwriting
			if (FileExist(file_path) && worldName != "")
			{
				FileHandle file_handle = OpenFile(file_path, FileMode.READ);
				string line_content = "";
				int lineCount;

				while (FGets(file_handle, line_content) >= 0)
				{
					lineCount++;
				}

				CloseFile(file_handle);

				// If line count is greater than 3, then we have already populated this default config or the server owner has made their own
				if (lineCount > 3)
				{
					Error("LINE COUNT: " + lineCount);
					return;
				}
			}

			FileHandle cFile = OpenFile(file_path, FileMode.WRITE); // WRITE will replace all contents.
			if (cFile != 0)
			{
				if (worldName == "ENOCH")
				{
					FPrintln(cFile, "// LIVONIA ICE LAKES");
					FPrintln(cFile, "static void ZenIceLakes_Spawn()");
					FPrintln(cFile, "{");
					FPrintln(cFile, "    // Flag specific ice planes to NOT spawn in the grid (useful for awkward shaped lakes or lakes with dam walls etc)");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11542.952148 261.838989 1974.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11534.952148 261.838989 1974.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11534.952148 261.838989 1998.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11534.952148 261.838989 2006.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11534.952148 261.838989 2014.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11542.952148 261.838989 2006.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11542.952148 261.838989 2014.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11542.952148 261.838989 2022.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11534.952148 261.838989 2022.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"2157.270020 468.899994 1352.530029\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"2157.270020 468.899994 1360.530029\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"8483.200195 265.252991 5138.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"8483.200195 265.252991 5130.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"8491.200195 265.252991 5138.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"8491.200195 265.252991 5130.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"8499.200195 265.252991 5138.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"8507.200195 265.252991 5138.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5570.479980 196.268341 11774.219727\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5562.479980 196.268341 11774.219727\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5578.479980 196.268341 11774.219727\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5586.479980 196.268341 11774.219727\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5586.479980 196.268341 11782.219727\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11251.389648 252.648453 12490.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11259.389648 252.648453 12490.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11251.389648 252.648453 12498.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11259.389648 252.648453 12498.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11251.389648 252.648453 12506.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11259.389648 252.648453 12506.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11251.389648 252.648453 12514.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11259.389648 252.648453 12514.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11251.389648 252.648453 12522.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11259.389648 252.648453 12522.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11251.389648 252.648453 12530.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11259.389648 252.648453 12530.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11251.389648 252.648453 12538.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11259.389648 252.648453 12530.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"736.719971 221.561386 8518.139648\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"736.719971 221.561386 8526.139648\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"736.719971 221.561386 8534.139648\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"736.719971 221.561386 8542.139648\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"736.719971 221.561386 8550.139648\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"736.719971 221.561386 8558.139648\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"590.729980 200.934860 7991.020020\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1123.439941 246.761993 5216.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1131.439941 246.761993 5216.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1139.439941 246.761993 5216.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1147.439941 246.761993 5216.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1147.439941 246.761993 5208.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1139.439941 246.761993 5208.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1131.439941 246.761993 5208.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1123.439941 246.761993 5208.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1139.439941 246.761993 5200.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1131.439941 246.761993 5200.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1123.439941 246.761993 5200.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1131.439941 246.761993 5192.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1123.439941 246.761993 5192.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1131.439941 246.761993 5184.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1123.439941 246.761993 5184.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1139.439941 246.761993 5184.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1139.439941 246.761993 5176.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1131.439941 246.761993 5176.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1123.439941 246.761993 5176.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1139.439941 246.761993 5168.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1131.439941 246.761993 5168.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1123.439941 246.761993 5168.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1123.439941 246.761993 5160.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1131.439941 246.761993 5160.339844\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5778.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4183.000000 258.072998 5778.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4183.000000 258.072998 5770.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5770.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4183.000000 258.072998 5762.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5762.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4183.000000 258.072998 5754.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5754.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4183.000000 258.072998 5746.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5746.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5738.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4183.000000 258.072998 5738.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5730.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5722.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5714.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5706.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5698.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4175.000000 258.072998 5690.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4191.000000 258.072998 5658.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4191.000000 258.072998 5666.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4199.000000 258.072998 5658.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4199.000000 258.072998 5666.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4199.000000 258.072998 5650.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4207.000000 258.072998 5658.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4215.000000 258.072998 5658.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4215.000000 258.072998 5666.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4207.000000 258.072998 5666.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4215.000000 258.072998 5674.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4207.000000 258.072998 5674.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4199.000000 258.072998 5674.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4207.000000 258.072998 5650.750000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"9749.589844 230.000000 7793.689941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"9757.589844 230.000000 7793.689941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"9765.589844 230.000000 7793.689941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"9765.589844 230.000000 7785.689941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"9773.589844 230.000000 7793.689941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"9773.589844 230.000000 7785.689941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"9781.589844 230.000000 7793.689941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12143.160156 224.139999 7884.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12151.160156 224.139999 7884.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12143.160156 224.139999 7892.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7884.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7892.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7900.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7908.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7916.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7924.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7932.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7940.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12143.160156 224.139999 7940.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7948.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12143.160156 224.139999 7948.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12151.160156 224.139999 7948.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7956.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7964.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7972.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12135.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12143.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12143.160156 224.139999 7972.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12143.160156 224.139999 7964.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12143.160156 224.139999 7956.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12151.160156 224.139999 7956.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12151.160156 224.139999 7964.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12151.160156 224.139999 7972.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12151.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12159.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12167.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12175.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12183.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12191.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12199.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12207.160156 224.139999 7980.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12191.160156 224.139999 7972.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12183.160156 224.139999 7972.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12175.160156 224.139999 7972.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12183.160156 224.139999 7964.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12175.160156 224.139999 7964.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12175.160156 224.139999 7956.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12159.160156 224.139999 7972.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12167.160156 224.139999 7972.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12159.160156 224.139999 7964.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12167.160156 224.139999 7964.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12167.160156 224.139999 7956.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"12159.160156 224.139999 7956.379883\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7651.169922 250.432999 5028.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7659.169922 250.432999 5028.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7667.169922 250.432999 5028.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7675.169922 250.432999 5028.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7651.169922 250.432999 5020.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7659.169922 250.432999 5020.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7667.169922 250.432999 5020.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7659.169922 250.432999 5012.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7651.169922 250.432999 5012.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7651.169922 250.432999 5004.189941\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7870.089844 262.576996 4695.060059\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"7862.089844 262.576996 4695.060059\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"6255.569824 336.682007 4501.709961\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"1951.449951 348.356995 2852.830078\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4395.910156 379.345001 3102.000000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"4395.910156 379.345001 3094.000000\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5648.330078 388.226013 1763.670044\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5640.330078 388.226013 1763.670044\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5648.330078 388.226013 1771.670044\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5648.330078 388.226013 1779.670044\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5648.330078 388.226013 1787.670044\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5648.330078 388.226013 1795.670044\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"5648.330078 388.226013 1803.670044\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11550.950195 261.838989 1966.800049\");");
					FPrintln(cFile, "    IGNORE_ICE_PLANE_POSITIONS.Insert(\"11550.950195 261.838989 1974.800049\");");
					FPrintln(cFile, "");
					FPrintln(cFile, "    // Spawn ice planes");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"1378.12 / 11758.98\", \"1478.52 / 11756.64\", \"1381.25 / 11690.23\", \"1483.59 / 11686.33\", 324.6);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"1710.55 / 11436.72\", \"1819.92 / 11441.41\", \"1713.67 / 11330.08\", \"1827.34 / 11330.47\", 302.958069);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"1954.69 / 12142.97\", \"2033.59 / 12146.09\", \"1957.81 / 12070.31\", \"2041.41 / 12072.66\", 324.327118);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"2025.39 / 10835.55\", \"2071.48 / 10830.86\", \"2033.20 / 10733.98\", \"2078.13 / 10737.11\", 262.385315);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"3148.83 / 11931.25\", \"3242.19 / 11935.94\", \"3157.03 / 11848.83\", \"3241.80 / 11844.92\", 249.516403);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"3685.16 / 12562.89\", \"3741.02 / 12559.77\", \"3687.11 / 12515.62\", \"3743.36 / 12516.02\", 234.599243);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"4082.81 / 10867.58\", \"4162.50 / 10863.67\", \"4088.28 / 10789.84\", \"4162.50 / 10791.02\", 214.339615);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"5538.48 / 11829.88\", \"5593.36 / 11828.52\", \"5547.27 / 11774.22\", \"5590.82 / 11776.95\", 196.268341);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"7190.23 / 11766.41\", \"7246.88 / 11767.19\", \"7201.37 / 11704.10\", \"7238.09 / 11703.32\", 175.996185);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"9621.09 / 12325.00\", \"9746.88 / 12314.84\", \"9636.72 / 12217.19\", \"9751.56 / 12216.41\", 199.062683);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"10724.41 / 11515.82\", \"10786.33 / 11516.60\", \"10726.37 / 11455.08\", \"10782.23 / 11456.64\", 172.216171);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"11251.39 / 12541.52\", \"11317.79 / 12540.09\", \"11259.45 / 12501.46\", \"11313.96 / 12490.83\", 252.648453);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"11660.55 / 11351.17\", \"11763.67 / 11358.59\", \"11664.45 / 11294.53\", \"11762.11 / 11294.53\", 176.087219);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"11736.33 / 9075.39\", \"11787.89 / 9075.78\", \"11741.80 / 8997.27\", \"11784.77 / 8998.05\", 187.596191);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"10909.38 / 9262.11\", \"10991.80 / 9266.80\", \"10910.16 / 9171.48\", \"10996.09 / 9173.05\", 184.057159);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"736.72 / 8586.72\", \"854.69 / 8592.97\", \"739.06 / 8495.31\", \"853.52 / 8494.14\", 221.561386);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"502.73 / 8117.19\", \"594.14 / 8127.34\", \"508.98 / 7995.70\", \"598.44 / 7991.02\", 200.934860);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"5672.66 / 9481.25\", \"5741.41 / 9482.03\", \"5678.13 / 9396.88\", \"5742.97 / 9395.31\", 184.178);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"6040.23 / 10332.81\", \"6075.00 / 10332.03\", \"6042.58 / 10283.59\", \"6076.95 / 10284.38\", 177.251);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"8085.94 / 9254.69\", \"8160.16 / 9266.41\", \"8089.06 / 9154.69\", \"8151.56 / 9154.69\", 265.489);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"3464.84 / 7417.19\", \"3527.73 / 7421.88\", \"3467.97 / 7333.98\", \"3532.81 / 7330.86\", 213.088);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"6807.03 / 8164.06\", \"6883.59 / 8160.16\", \"6807.81 / 8094.53\", \"6882.03 / 8095.31\", 208.241);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"5796.48 / 7710.16\", \"5834.77 / 7717.58\", \"5796.09 / 7644.92\", \"5833.59 / 7647.27\", 225.386);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"8052.73 / 8554.30\", \"8113.67 / 8557.81\", \"8061.72 / 8453.52\", \"8114.06 / 8454.69\", 236.627);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"9374.22 / 8712.50\", \"9459.38 / 8705.47\", \"9375.00 / 8649.22\", \"9459.38 / 8647.66\", 223.015);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"1123.44 / 5211.72\", \"1307.81 / 5217.19\", \"1135.94 / 4952.34\", \"1309.37 / 4956.25\", 246.762);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"4175.00 / 5772.66\", \"4263.28 / 5782.81\", \"4189.84 / 5619.53\", \"4267.97 / 5618.75\", 258.073);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"5342.97 / 7004.69\", \"5426.56 / 7011.72\", \"5347.66 / 6934.38\", \"5423.44 / 6935.94\", 249.412);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"8255.47 / 7060.16\", \"8303.91 / 7064.06\", \"8259.77 / 6996.09\", \"8302.34 / 6998.44\", 260.817);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"8413.28 / 7920.31\", \"8488.67 / 7926.17\", \"8414.06 / 7860.55\", \"8489.06 / 7859.77\", 264.675);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"9733.59 / 7789.06\", \"9788.28 / 7796.09\", \"9738.28 / 7732.03\", \"9787.50 / 7729.69\", 230.0);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"11278.13 / 7706.64\", \"11339.84 / 7709.38\", \"11280.47 / 7654.30\", \"11340.23 / 7652.73\", 194.571);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"12135.16 / 7975.00\", \"12300.75 / 7985.16\", \"12139.06 / 7884.38\", \"12300.41 / 7886.72\", 224.14);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"6519.53 / 9069.53\", \"6649.22 / 9075.78\", \"6528.13 / 8801.56\", \"6662.50 / 8801.56\", 203.515);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"8047.66 / 5525.00\", \"8147.66 / 5535.16\", \"8053.13 / 5434.38\", \"8146.88 / 5432.03\", 241.938);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"7651.17 / 5033.59\", \"7756.64 / 5029.69\", \"7659.38 / 4892.19\", \"7758.20 / 4899.22\", 250.433);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"7846.09 / 4698.44\", \"7894.53 / 4694.53\", \"7850.00 / 4639.06\", \"7889.06 / 4645.31\", 262.577);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"6926.56 / 4586.91\", \"7014.84 / 4587.89\", \"6934.96 / 4504.10\", \"7016.80 / 4501.95\", 273.261);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"12267.19 / 4792.19\", \"12619.14 / 4767.58\", \"12314.06 / 4521.88\", \"12627.34 / 4533.98\", 226.077);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"1136.72 / 3896.09\", \"1182.81 / 3894.92\", \"1138.28 / 3843.75\", \"1182.81 / 3845.70\", 295.923);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"1899.22 / 4560.16\", \"1950.00 / 4558.98\", \"1905.86 / 4513.67\", \"1950.39 / 4514.06\", 265.985);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"6469.53 / 4651.95\", \"6555.47 / 4653.13\", \"6472.27 / 4619.92\", \"6558.20 / 4619.53\", 307.582);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"6159.57 / 4513.09\", \"6253.52 / 4517.19\", \"6165.82 / 4456.64\", \"6261.72 / 4453.71\", 336.682);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"12079.69 / 3896.09\", \"12180.08 / 3904.69\", \"12083.59 / 3825.78\", \"12185.55 / 3822.66\", 214.238);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"1839.45 / 2852.34\", \"1958.98 / 2858.59\", \"1844.53 / 2748.83\", \"1953.91 / 2750.78\", 348.357);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"3738.28 / 3028.91\", \"3786.72 / 3026.56\", \"3742.19 / 2987.50\", \"3789.84 / 2988.28\", 370.423);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"9357.81 / 3514.06\", \"9397.66 / 3513.67\", \"9363.09 / 3462.70\", \"9395.70 / 3464.45\", 279.555);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"10503.13 / 3082.81\", \"10533.98 / 3081.84\", \"10503.52 / 3049.61\", \"10535.16 / 3049.80\", 265.151);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"11031.64 / 3787.50\", \"11085.55 / 3800.00\", \"11034.77 / 3723.44\", \"11085.55 / 3722.27\", 285.248);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"6825.39 / 3321.09\", \"6971.48 / 3325.00\", \"6829.69 / 3217.58\", \"6976.56 / 3212.50\", 297.65);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"4203.91 / 3105.47\", \"4399.22 / 3102.34\", \"4204.69 / 2950.00\", \"4397.66 / 2952.34\", 379.345);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"10545.31 / 1688.67\", \"10618.36 / 1686.33\", \"10552.34 / 1584.77\", \"10619.53 / 1586.72\", 234.168);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"10578.52 / 1450.00\", \"10633.59 / 1450.39\", \"10581.64 / 1365.23\", \"10637.50 / 1367.97\", 231.69);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"10706.25 / 951.17\", \"10798.05 / 944.92\", \"10710.55 / 877.34\", \"10798.44 / 880.08\", 226.472);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"11328.91 / 1107.03\", \"11444.53 / 1107.03\", \"11339.84 / 987.50\", \"11446.88 / 989.06\", 227.352);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"2407.81 / 1392.19\", \"2450.00 / 1389.06\", \"2409.37 / 1309.38\", \"2450.78 / 1310.16\", 434.642);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"3411.72 / 891.80\", \"3461.72 / 888.28\", \"3416.41 / 838.67\", \"3462.11 / 842.97\", 387.728);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"5536.33 / 1834.77\", \"5648.05 / 1828.52\", \"5540.63 / 1763.67\", \"5650.00 / 1765.23\", 388.226);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"11534.95 / 1994.15\", \"11542.58 / 1966.80\", \"11596.28 / 2040.62\", \"11655.86 / 1972.66\", 261.839);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"2441.99 / 6621.29\", \"2478.32 / 6620.70\", \"2446.48 / 6586.13\", \"2476.56 / 6586.72\", 284.529);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"3373.05 / 6769.53\", \"3415.23 / 6767.58\", \"3379.30 / 6681.25\", \"3415.62 / 6683.20\", 224.403);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"10168.75 / 6029.69\", \"10382.03 / 6034.38\", \"10174.22 / 5828.91\", \"10372.66 / 5828.91\", 216.548);");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"626.56 / 2445.90\", \"616.21 / 2321.68\", \"704.69 / 2444.92\", \"699.80 / 2328.71\", 320.439);");
					FPrintln(cFile, "");
					FPrintln(cFile, "    // Dambog dam wall lake (requires special treatment)");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"1997.27 / 1508.98\", \"2157.42 / 1514.45\", \"2007.42 / 1344.53\", \"2159.77 / 1346.09\", 468.9);");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"2159.639893 468.9 1380.930054\", \"0.000000 0.000000 0.000000\");");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"2157.659912 468.9 1372.939941\", \"0.000000 0.000000 0.000000\");");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"2156.449951 468.9 1364.949951\", \"0.000000 0.000000 0.000000\");");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"2155.469971 468.9 1356.949951\", \"0.000000 0.000000 0.000000\");");
					FPrintln(cFile, "");
					FPrintln(cFile, "    // Karlin dam wall lake (requires special treatment)");
					FPrintln(cFile, "    SpawnZenIcePlanes(\"8483.20 / 5142.19\", \"8577.34 / 5144.14\", \"8483.98 / 5034.38\", \"8585.16 / 5037.11\", 265.253);");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"8493.209961 265.252991 5127.060059\", \"-34.199993 0.000000 0.000000\");");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"8512.669922 265.252991 5140.270020\", \"-34.199993 0.000000 0.000000\");");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"8506.209961 265.252991 5135.990234\", \"-34.199993 0.000000 0.000000\");");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"8499.799805 265.252991 5131.560059\", \"-34.199993 0.000000 0.000000\");");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"8511.959961 265.252991 5130.399902\", \"-34.199993 0.000000 0.000000\");");
					FPrintln(cFile, "");
					FPrintln(cFile, "    // Zalesie dam wall (requires special treatment)");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"1135.410034 246.761993 5157.680176\", \"-48.599998 0.000000 0.000000\");");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"1140.640015 246.761993 5163.649902\", \"-48.599998 0.000000 0.000000\");");
					FPrintln(cFile, "    ZenIceLakes_SpawnObject(\"Land_ZenIceSheet_4x4\", \"1145.839966 246.761993 5169.549805\", \"-48.599998 0.000000 0.000000\");");
					FPrintln(cFile, "}");

					CloseFile(cFile);
					return;
				}

				string errorMsg = "Error! This map " + worldName + " does not have a default config. Check the steam workshop mod page to see if anyone has made one for this map.";
				if (worldName != "")
					FPrintln(cFile, "// " + errorMsg);
				FPrintln(cFile, "static void ZenIceLakes_Spawn()");
				FPrintln(cFile, "{");
				if (worldName != "")
					FPrintln(cFile, "    Error(\"[ZenIceLakes] " + errorMsg + "\");");
				FPrintln(cFile, "}");
				CloseFile(cFile);
			}
		}
	}
}

modded class DayZGame
{
	void DayZGame()
	{
#ifdef SERVER
		ZenIcePlanesDefault.CheckDefaultCFile();
#endif
	}
}