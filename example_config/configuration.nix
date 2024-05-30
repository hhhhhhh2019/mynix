default_version = "unstable" # unstable, release, stable


{
	repos = {
		os = {
			url = "file:///home/alex/progs/pkg/repos/os";
			inherit default_version;
		};
	};

	config = { os }: {
		system.hostName = "qwertyuiop";
		system.timeZone = "Europe/Moscow";
		# system.defaultLocale = "ru_RU.UTF-8"; пока непонятно, как это должно работать
	} done
}
