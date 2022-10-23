#include <string>
#include <string.h>
#include <map>
#include <vector>




namespace tidy {


class FLAG {
private:
	bool set;
	std::string value;
public:
	bool isSet(){ return set; }
	std::string getValue(){ return value; }
	FLAG(bool set, std::string value) : set {set}, value {value} {}
};


struct ArgvInput {
	std::map<std::string, FLAG*> flags;
	std::vector<std::string> params;
};


struct parameters {
	int required;
	int optional;
};

struct cmd_flag {
	std::string name;
	std::string param;
};

struct flag_struct  {
	std::string name;
	std::string alias;
	bool required_param;
	std::string help;
};









class OPTION {

private:

	void (*perform) (ArgvInput);
	std::vector<flag_struct> flags;
	std::map<std::string, flag_struct> flags_map;
	std::map<std::string, std::string> flags_aliases;
	bool exec;
	parameters params;
	std::string help;

	void init();

public:
	
	std::map<const std::string, OPTION*> options;


	OPTION ( 	void (*perform) (ArgvInput), 
		    	std::vector<flag_struct>				flags, 
		    	parameters 								params,
		    	std::map<const std::string, OPTION*> 	options,
		    	std::string 							help
		   ) : 
		   		perform		{perform},
		   		flags 		{flags},
		   		options 	{options},
		   		params 		{params},
		   		help 		{help} 
		   		{init();}

	OPTION ( 	void (*perform) (ArgvInput), 
		    	std::vector<flag_struct>				flags, 
		    	parameters 								params,
		    	std::string 							help
		   ) : 
		   		perform		{perform},
		   		flags 		{flags},
		   		params      {params},
		   		options 	{},
		   		help 		{help} 
		   		{init();}


	OPTION ( 	void (*perform) (ArgvInput),  
		    	std::vector<flag_struct>				flags, 
		    	std::map<const std::string, OPTION*> 	options,
		    	std::string 							help
		   ) : 
		   		perform		{perform},
		   		flags 		{flags},
		   		params      {parameters{0,0}},
		   		options 	{options},
		   		help 		{help} 
		   		{init();}

	OPTION ( 	void (*perform) (ArgvInput),  
		    	std::vector<flag_struct>				flags, 
		    	std::string 							help
		   ) : 
		   		perform		{perform},
		   		flags 		{flags},
		   		params      {parameters{0,0}},
		   		options 	{},
		   		help 		{help} 
		   		{init();}



	OPTION ( 	 
				std::map<const std::string, OPTION*> 	options,
				std::string 							help
		   ) : 
		   		perform 	{NULL},
		   		flags 		{},
		   		params      {parameters{0,0}},
		   		options 	{options},
		   		help 		{help} 
		   		{init();}


	std::string get_help();

	bool run(std::vector<cmd_flag>, std::vector<std::string>);

	std::map<std::string, std::string>	validate_flags(std::vector<cmd_flag>);
	std::map<std::string, FLAG*> 		normalize_flags(std::map<std::string, std::string>);
	std::vector<std::string> 			validate_params(std::vector<std::string>);
	
	int get_required(){ return this->params.required; }
	int get_optional(){ return this->params.optional; }

	std::string getNameFromAlias(std::string);
	std::vector<flag_struct> get_flags() { return flags; }

};












std::map<const std::string, OPTION*> OptionsList(std::map<const std::string, OPTION*> list) {
	return list;
}

flag_struct Flag(std::string name, bool required_param, std::string help){
	return {name, "", required_param, help};
}

flag_struct Flag(std::string name, std::string alias, bool required_param, std::string help){
	return {name, alias, required_param, help};
}

parameters Params(int req)		   { return {req, 0  }; }   //prevent nulls empty ""
parameters Params(int req, int opt){ return {req, opt}; }



void printError(short int err_code, std::string value){
	switch (err_code){
		case 1: std::cout << "Error: Próba przypisania parametru do pustej flagi  (" << value << ")" << std::endl; break;
		case 2: std::cout << "Warning: Zignorowano flagę niebędącą literą - flaga może być literą np. -f lub tekstem np. --flag  (" << value << ")" << std::endl; break;
		case 3: std::cout << "Warning: Zignorowano flagę umieszczoną w parametrach  (" << value << ")" << std::endl; break;
		case 4: std::cout << "Warning: Zignorowano flagę nadpisującą wartość  (" << value << ")" << std::endl; break;
		case 5: std::cout << "Warning: Zignorowano flagę powtórzoną  (" << value << ")" << std::endl; break;
		case 6: std::cout << "Warning: Zignorowano flagę bez parametru, zastąpiono nowym (" << value << ")" << std::endl; break;
		case 7: std::cout << "Warning: Zignorowano flagę niepoprawną  (" << value << ")" << std::endl; break;
		case 8: std::cout << "Warning: Zignorowano flagę pustą" << std::endl; break;
	}
}


		 
int findFlag( std::vector<cmd_flag> * vec, std::string name) {
	for(auto it = vec->begin(); it != vec->end(); it++)
			if(it->name == name)
				return it - vec->begin();
	return -1;
}  






	
bool ArgvRouter(int argc, char * argv[], OPTION * menu_options) {

	short int params = 0;
	std::vector<std::string> parameters;
	OPTION * target = menu_options;
	std::vector<cmd_flag> flags;



	for(int i = 1; i < argc; i++){
		if(strlen(argv[i]) > 2 && argv[i][0]=='-' && argv[i][1]=='-'){	     //if string flag	(--flag)
			const char * equal_char = strchr(argv[i], '=');
			if(equal_char == NULL || (strlen(argv[i]) - strlen(equal_char)) > 2){
				std::string tmp_name = std::string(argv[i]).substr(2);
				std::string tmp_value = "";
				if(equal_char != NULL){                                        // parameter set
					tmp_value = std::string(argv[i]).substr((strlen(argv[i]) - strlen(equal_char) + 1));
					tmp_name = std::string(argv[i]).substr(2, (strlen(argv[i]) - strlen(equal_char))-2);
				}
				tmp_name = target->getNameFromAlias(tmp_name);
				int search = findFlag(&flags, tmp_name);
				if(search < 0)
					flags.push_back({tmp_name, tmp_value});
				else if(tmp_value != "" && flags.at(search).param == ""){
					flags.at(search).param = tmp_value;
					printError(6, argv[i]);
				} else if(tmp_value != "")
					printError(4, argv[i]);
				else printError(5, argv[i]);
			} else printError(1, argv[i]);
		} else if(argv[i][0]=='-' && strlen(argv[i]) > 1){											     //if one char flag/flags (-f)
			const char * equal_char = strchr(argv[i], '=');	
			if(equal_char == NULL || (strlen(argv[i]) - strlen(equal_char)) > 1){
				std::string param = "";
				int iterate_to = strlen(argv[i]);
				if(equal_char != NULL){
					param = std::string(argv[i]).substr((strlen(argv[i]) - strlen(equal_char) + 1));
					iterate_to = (strlen(argv[i]) - strlen(equal_char));
				}
				for(int j = 1; j < iterate_to; j++){
					if(isalpha(argv[i][j])){ 												  // ignoruje nienumeryczne -p12
						std::string tmp_name = target->getNameFromAlias(std::string(1, argv[i][j]));
						int search = findFlag(&flags, tmp_name);

						if(j + 1 == iterate_to){
							if(search < 0)
								flags.push_back({ tmp_name, param});
							else if(param != "" && flags.at(search).param == ""){
								flags.at(search).param = param;
								printError(6, argv[i]);
							}
							else if(param != "")
								printError(4, argv[i]);
							else 
								printError(5, std::string(1, argv[i][j]));
						} else { 
							if(search < 0)
								flags.push_back({tmp_name, ""});
							else printError(5, std::string(1, argv[i][j]));
						}
					} else printError(2, argv[i]);
				}
			} else printError(1, argv[i]);
		} else if(argv[i][0]=='-'){
			 printError(8, "");
		} else if(target->options.find(argv[i]) != target->options.end()){	     //if it's argument		
				target=target->options[argv[i]];
		} else   										                         // if  parameters
			for(; i < argc; i++) {
				if(argv[i][0] == '-')
					printError(3, argv[i]);
				else
					parameters.push_back(argv[i]);
			}
	}

	//sprawdź flagi
	try {
		target->run(flags, parameters);
	} catch(std::string  ex){
		std::cout << ex << std::endl;
		std::cout << target->get_help() << std::endl;
		return 1;
	}


	
		
	return 0;
}
















void OPTION::init(){
	for(auto it : this->flags){
		if(it.alias.length()){
			this->flags_aliases[it.alias] = it.name;
			this->flags_map[it.name] = it;
		}else
			this->flags_map[it.name] = it;
	}
}



std::string OPTION::getNameFromAlias(std::string name) {
	auto it = this->flags_aliases.find(name);
	if(it != this->flags_aliases.end())
		return it->second;
	else return name;
}

bool OPTION::run(std::vector<cmd_flag> flags, std::vector<std::string> params){
	auto v_flags = this->validate_flags(flags);
	auto n_flags = this->normalize_flags(v_flags);
	auto v_params = this->validate_params(params);
	if(this->perform != NULL && v_params.size() >= this->params.required) {
		this->perform({n_flags, v_params});
		return true;
	}
	else return false;
}



std::string OPTION::get_help(){ 
	std::string helper;
	if(this->flags.size()){
		int max_length = 0;
		int flags_space = 2;

		for(auto it : this->flags)
			if(it.alias.length() > max_length)
				max_length = it.alias.length();
		
		if(max_length > 0)
			max_length += flags_space + 2;

		helper += this->help + "\n\nFlags:\n";
		for(auto it : this->flags){               
			helper += " -" + it.name;
			int tmp_size = max_length;

			if(it.alias.length()  >0) {
				helper +=  std::string(flags_space, ' ') + "--" + it.alias;  
				tmp_size -= (it.alias.length() + flags_space + 2);
			}
			helper += std::string(tmp_size, ' ') + "  - "+ it.help +"\n";
		}
	}

	if(this->options.size()){
		helper += + "\nParams:\n";
		for(auto it : this->options)
			helper += it.first + "   - " + it.second->help + "\n";
	}

	return helper; 
}

std::map<std::string, FLAG*>  OPTION::normalize_flags(std::map<std::string, std::string> validated_flags){
	std::map<std::string, FLAG*> flags_normalized;
	for (auto flag : this->flags){
		bool is_flag_set = validated_flags.find(flag.name) != validated_flags.end();
		flags_normalized[flag.name] = new FLAG(is_flag_set,  is_flag_set ? validated_flags[flag.name] : "");
	}
	return flags_normalized;
}


std::map<std::string, std::string>  OPTION::validate_flags(std::vector<cmd_flag> flags){  
	std::map<std::string, std::string> flags_validated;


	for(auto flag : flags) {
		if(this->flags_map.find(flag.name) != this->flags_map.end()){
			if(this->flags_map.find(flag.name)->second.required_param && flag.param == "")
				std::cout << "Error: Brak wymaganego parametru dla flagi  (" <<  flag.name << "). Zignorowano flagę." << std::endl;
			else
				flags_validated[flag.name] = flag.param;
		}else  std::cout << "Warning: Zignorowano flagę niepoprawną  (" <<  flag.name << ")" << std::endl;	
	}
	return flags_validated;
}


std::vector<std::string> OPTION::validate_params(std::vector<std::string> params){
	std::vector<std::string> tmp_params;

	if(params.size() < this -> params.required){
		std::cout << "Error: Zbyt mało parametrów." << std::endl;
		std::cout << this -> get_help();
	} else{
		for(auto it : params){
			if(tmp_params.size() < (this->params.required+ this->params.optional))
				tmp_params.push_back(it);
			else
				std::cout << "Warning: Zignorowano parametr nadmiarowy ("<< it << ")" << std::endl;
		}
	}

	
	return tmp_params;
}








}