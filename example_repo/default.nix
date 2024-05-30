setup = {}: {
	
} done


config = { config }:
	result = []

	return result
done


{
	name = "test";

	inherit setup;
	inherit config;
}
