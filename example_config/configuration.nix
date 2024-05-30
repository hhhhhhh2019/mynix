default_version = "lastest" # lastest, release, stable


{}: {
	repos = {
		os = {
			url = "github:mynix/os_repo";
			inherit default_version;
		};

		base = {
			url = "github:mynix/base_repo";
			inherit default_version;
		};
	};

	config = { os, base }: {
		system.hostName = "qwertyuiop";
		system.timeZone = "Europe/Moscow";
		system.defaultLocale = "ru_RU.UTF-8";

		import "./modules/*";

		users = {
			user = {
				home = "/home/user";
				shell = base.pkgs.zsh;
				groups = [ "wheel", "uucp" ];
				import "./users/user/*";
			};
		};
	} done
} done
