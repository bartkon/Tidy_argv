#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include "Option.cpp"




std::map<const std::string, Option*> OptionsList(std::map<const std::string, Option*> list) {
	return list;
}


bool ArgvRouter(int argc, char * argv[], Option * menu_options) {

	short int params = 0;
	std::vector<std::string> parameters;
	Option * target = menu_options;
	std::vector<std::string> flags;
	
	for(int i = 1; i < argc; i++){
		if(strlen(argv[i]) > 2 && argv[i][0]=='-' && argv[i][1]=='-'){			 //if string flag	(--flag)
				flags.push_back(argv[i]+2);
		} else if(argv[i][0]=='-'){							                     //if one char flag/flags (-f)
			for(int j=1; j < strlen(argv[i]); j++)
				flags.push_back(std::string(1, argv[i][j]));
		} else if(target->options.find(argv[i]) != target->options.end()){	     //if it's argument		
				target=target->options[argv[i]];
		} else   										                         // if  parameters
			for(; i < argc; i++) 
				parameters.push_back(argv[i]);
	}


	bool result = target->run(flags, parameters);
	if(!result)
		std::cout << target->get_help() << std::endl;
	return result;
}




void add_func(std::vector<std::string> flags, std::vector<std::string> params){

	std::cout << "-> add \nFlags: ";
	for(int i = 0; i < flags.size(); i++)
		std::cout<< "["<<i<<"]"<<flags.at(i);
	std::cout << "\nParams: ";
	for(int i = 0; i < params.size(); i++)
		std::cout<< "["<<i<<"]"<<params.at(i);

	std::cout<< std::endl;
	std::cout<< stoi(params.at(0)) + stoi(params.at(1)) << std::endl;
}



int main(int argc, char * argv[])
{
	Option menu(
		OptionsList({

			{"add", new Option(
				add_func,
				{{"d","Debug info",0},{"m","Math logic",0}},
				2,
				0,
				std::map<const std::string, Option*> {},
				"adds two digits as parameters. USE: ./app add <first number> <second number>"
				)},

			{"sub", new Option(
				[](std::vector<std::string> flags, std::vector<std::string> params){
					std::cout << "sub: "<< std::endl;
				},
				{{"d","Debug info",0},{"m","Math logic",0}},
				std::map<const std::string, Option*> {},
				"substract two digits given as parameters. USE: ./app sub <first number> <second number>"
				)}
		}),
		"Use: ./app <flags> [param]"
	);


	
	

    return !ArgvRouter(argc, argv, &menu);
}


