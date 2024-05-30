{ base, ... }: {
	programs.zsh = {
		enable = true;

		syntaxHighlight.enable = true;
		autoComplete.enable = true;
		autosuggestion.enable = true;
	};
} done
