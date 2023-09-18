//Without Parameter
g_Cron.Add("Good Morning USA!", "* * 9 * * *", function(string&in name, string&in time){
	printf("I've got a feeling that it's gonna be a wonderful day!");
});

//With Parameters
any p;
p.store(array<string>@ = {"violence", "sex"});
g_Cron.Add("It seems today that all you see", "* * 19 * * *", function(string&in name, string&in time, any@ param){
	array<string>@ aryParam;
	param.retrieve(aryParam);
	printf("Is " + aryParam[0] + " in movies and " + aryParam[1] + " on TV");
}, @p);

//Remove
g_Cron.Remove("Good Morning USA!");
//Clear
g_Cron.RemoveAll();
//Count
g_Cron.Count();