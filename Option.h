

struct flag_struct  {
	std::string name;
	std::string help;
	short int required;
};




class Option {
private:

	void (*perform) (std::vector<std::string>, std::vector<std::string>);
	std::vector<flag_struct> flags;
	bool exec;
	int required;
	int optional;
	

public:
	std::string help;
	std::map<const std::string, Option*> options;


	Option ( 	void (*perform) (std::vector<std::string>, std::vector<std::string>), 
		    	std::vector<flag_struct>									flags, 
		    	int 														required,
		    	int 														optional,
		    	std::map<const std::string, Option*> 						options,
		    	std::string 												help
		   ) : 
		   		perform		{perform},
		   		flags 		{flags},
		   		options 	{options},
		   		required 	{required},
		   		optional 	{optional},
		   		help 		{help} 
		   		{}


	Option ( 	void (*perform) (std::vector<std::string>, std::vector<std::string>),  
		    	std::vector<flag_struct>									flags, 
		    	std::map<const std::string, Option*> 						options,
		    	std::string 												help
		   ) : 
		   		perform		{perform},
		   		flags 		{flags},
		   		options 	{options},
		   		required 	{0},
		   		optional 	{0},
		   		help 		{help} 
		   		{}


	Option ( 	 
				std::map<const std::string, Option*> 						options,
				std::string 												help
		   ) : 
		   		perform 	{NULL},
		   		flags 		{{}},
		   		options 	{options},
		   		required 	{0},
		   		optional 	{0},
		   		help 		{help} 
		   		{}


	std::string get_help();
	bool run(std::vector<std::string> flags, std::vector<std::string> params);

	int get_required(){ return this->required; }
	int get_optional(){ return this->optional; }
	int get_max_params() { return this->required + this->optional; }
	std::vector<flag_struct> get_flags() { return flags; }

	

};
