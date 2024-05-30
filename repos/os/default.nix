os = {
	pkgs = {
		base = "pkgs/base.nix"
	};
}


setup = {}: os done


config = { config }:
	result = []

	stdlib.append(result, import "./pkgs/base.nix" (os, config) )

	return result
done


{
	inherit setup;
	inherit config;
}
