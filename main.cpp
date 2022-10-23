#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
#include "Tidy_argv.cpp"








bool isStringValidFloat(std::string text){
	try {
		float tmp = std::stof(text);
		return true;
 	} catch(...) {
        return false;
 	}
}


void sub_func(tidy::ArgvInput input){
	try {
		float a = std::stof(input.params.at(0));
		float b = std::stof(input.params.at(1));
		
		if(input.flags["m"]->isSet())
			std::cout << a << " - " << b << " = " ;
		std::cout << a + b << std::endl;
	} catch(...) {
		throw std::string("Invalid numerical parameters."); 
	}
}


void add_func(tidy::ArgvInput input){

	try {
		float a = std::stof(input.params.at(0));
		float b = std::stof(input.params.at(1));
		

		// debug
		std::cout << "Flags: ";
		for(auto flag : input.flags)
			if(flag.second->isSet())
				std::cout << flag.first << "(" << flag.second->getValue() << ") ";
		std::cout << "\nParams: ";
		for(int i = 0; i < input.params.size(); i++)
			std::cout<< "["<<i<<"]"<< input.params.at(i);
		std::cout << std::endl;
		// debug

		if(input.flags["m"]->isSet())
			std::cout << a << " + " << b << " = " ;
		std::cout << a + b << std::endl;
	} catch(...) {
		throw std::string("Invalid numerical parameters. ("+input.params.at(0)+", "+input.params.at(1)+")"); 
	}
}



int main(int argc, char * argv[])
{
	auto menu = new tidy::OPTION (
		tidy::OptionsList({

			{"add", new tidy::OPTION(
				add_func,
				{	
					tidy::Flag("m", "math",	 	false, 	"Math logic"),
					tidy::Flag("s", "sqrt", 	true, 	"propably logic")
				},
				tidy::Params(2),
				"adds two digits as parameters. USE: ./app add <first number> <second number>"
			)},

			{"sub", new tidy::OPTION(
				sub_func,
				{
					tidy::Flag("m", "math", false, "Math logic")
				},
				tidy::Params(2),
				"substract two digits given as parameters. USE: ./app sub <first number> <second number>"
			)}

		}),
		"Use: ./app <flags> [param]"
	);




    return !tidy::ArgvRouter(argc, argv, menu);
}


