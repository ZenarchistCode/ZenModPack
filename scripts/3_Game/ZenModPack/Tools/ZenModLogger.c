class ZenModLogger
{
	const static string LOG_FILE = "ZenMod";
	static string LOG_FOLDER = "$profile:\\Zenarchist\\Logs\\";
	static string DATE_FORMAT = "";

	// Log timer
	private static ref Timer m_PvpLogTimer;
	private static ref array<string> PVP_LOG;

	void ZenModLogger()
	{
		if (!ZenModEnabled("ZenModLogger"))
			return;

		// Create overall log folder
		if (!FileExist(LOG_FOLDER))
		{
			MakeDirectory(LOG_FOLDER);
		}

		// Get folder date
		int serverYear = 0;
		int serverMonth = 0;
		int serverDay = 0;
		GetYearMonthDayUTC(serverYear, serverMonth, serverDay);

		string yearStr = serverYear.ToString();
		string monthStr = "";
		string dayStr = "";

		if (serverMonth <= 9)
			monthStr = "0" + serverMonth.ToString();
		else
			monthStr = serverMonth.ToString();

		if (serverDay <= 9)
			dayStr = "0" + serverDay.ToString();
		else
			dayStr = serverDay.ToString();

		DATE_FORMAT = dayStr + "-" + monthStr + "-" + yearStr;

		// Create today's log folder
		LOG_FOLDER = LOG_FOLDER + "/" + DATE_FORMAT + "/";
		if (!FileExist(LOG_FOLDER))
		{
			MakeDirectory(LOG_FOLDER);
		}

		// Prepare pvp timer to log every 30 secs
		if (!GetGame().IsClient() && GetZenUtilitiesConfig().ShouldLogPVP)
		{
			PVP_LOG = new array<string>;
			m_PvpLogTimer = new Timer();
			m_PvpLogTimer.Run(30, this, "LogDelayedPVP", NULL, true);
		}
		
		Log("Delayed PVP Log Timer Started", "pvp");
	}

	// Prints directly to Log folder without date organization
	static void LogEx(string txt, string type = "general")
	{
		if (!ZenModEnabled("ZenModLogger"))
			return;

		string file_path = "$profile:\\Zenarchist\\Logs\\" + LOG_FILE;

		switch (type)
		{
			case "performance":
				file_path = file_path + "_performance.log";
				break;
			case "crash":
				file_path = file_path + "crash.log";
				break;
			case "general":
				file_path = file_path + "_general.log";
				break;
			default:
				file_path = file_path + "_" + type + ".log";
				break;
		}

		FileHandle logFile = OpenFile(file_path, FileMode.APPEND);
		if (logFile != 0)
		{
			FPrintln(logFile, ZMGetDate() + " | " + txt);
			CloseFile(logFile);
		}
	}

	// Prints to specified log file, organized by date folder, includeDate = include timestamp in print line
	static void Log(string txt, string type = "general", bool includeDate = true)
	{
		if (!ZenModEnabled("ZenModLogger"))
			return;

		string file_path = LOG_FOLDER + LOG_FILE;

		switch (type)
		{
			case "pvp":
				file_path = file_path + "_pvp.log";
				break;
			case "basebuild":
				file_path = file_path + "_basebuild.log";
				break;
			case "baseraid":
				file_path = file_path + "_baseraid.log";
				break;
			case "login":
				file_path = file_path + "_logins.log";
				break;
			case "vehicle":
				file_path = file_path + "_vehicle.log";
				break;
			case "mission":
				file_path = file_path + "_mission.log";
				break;
			case "notes":
				file_path = file_path + "_notes.log";
				break;
			case "quests":
				file_path = file_path + "_quests.log";
				break;
			case "trader":
				file_path = file_path + "_trader.log";
				break;
			case "performance":
				file_path = file_path + "_performance.log";
				break;
			case "general":
				file_path = file_path + "_general.log";
				break;
			default:
				file_path = file_path + "_" + type + ".log";
				break;
		}

		if (file_path == "gunfight")
		{
			PVP_LOG.Insert(txt);
			return;
		}

		FileHandle logFile = OpenFile(file_path, FileMode.APPEND);
		if (logFile != 0)
		{
			if (includeDate)
			{
				FPrintln(logFile, ZMGetDate() + " | " + txt);
				ZMPrint(ZMGetDate() + " | " + txt);
			} 
			else
			{
				FPrintln(logFile, txt);
				ZMPrint(txt);
			}

			CloseFile(logFile);
		}
	}

	static void LogDelayedPVP()
	{
		string file_path = LOG_FOLDER + LOG_FILE + "_pvp.log";
		FileHandle logFile = OpenFile(file_path, FileMode.APPEND);
		if (logFile == 0)
			return;

		foreach(string s : PVP_LOG)
		{
			if (logFile != 0)
			{
				FPrintln(logFile, ZMGetDate() + " " + s);
			}
		}

		CloseFile(logFile);
		PVP_LOG.Clear();
	}
}

static void ZMPrint(Object obj)
{
	Print("[ZEN|" + ZMGetDate() + "] " + obj.ToString());
}

static void ZMPrint(string s)
{
	Print("[ZEN|" + ZMGetDate() + "] " + s);
}

static string ZMGetDate(bool fileFriendly = false)
{
	int year, month, day, hour, minute, second;

	GetYearMonthDay(year, month, day);
	GetHourMinuteSecond(hour, minute, second);

	string date = day.ToStringLen(2) + "." + month.ToStringLen(2) + "." + year.ToStringLen(4) + " " + hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);
	if (fileFriendly)
	{
		date.Replace(" ", "_");
		date.Replace(".", "-");
		date.Replace(":", "-");
	}

	return date;
}