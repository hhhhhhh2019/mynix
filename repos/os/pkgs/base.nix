name = "os base package"

unstable = "1"
release = "1"
stable  = "1"

{ os, config }:
	versions = [
		{
			inherit name;
			version = "1";

			src = fetchurl({
				url = "file:///home/alex/progs/pkg/repos/os/zip/base_os_1.tar.gz",
				hash = "461202bb45023e23313f77b5bf8d28fb0d51876e0a30bc746e23b6b4a8d3db7d"; # sha256
			});
			dependencies = [];
			build_dependencies = [];

			preBuild = "";
			build = "echo build in $src";
			postBuild = "";

			preInstall = "";
			install = ''
				echo install from $src to $outпередаются аргументы функции, выполняемой в данный момент.

				cp $src/* $out/ -R

				cat << EOF > $out/etc/hostname
				${config.hostName}
				EOF

				ln -sf $out/usr/share/${config.timeZone} $out/etc/localtime
			'';
			postInstall = "";

			hooks = {};
		},
	]


	version = if config.os.base_os.version then config.os.base_os.version else os.default_version

	if version == "unstable" then
		version = unstable
	else if version == "release" then
		version = release
	else if version == "stable" then
		version = stable
	done

	return stdlib.array_get(versions, version)
done
