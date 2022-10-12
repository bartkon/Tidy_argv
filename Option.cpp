#include "Option.h"
#include <string>
#include <string.h>
#include <map>
#include <vector>




	bool Option::run(std::vector<std::string> flags, std::vector<std::string> params){
		if(this->perform != NULL && (params.size() >= this->get_required()) && (params.size() <= (this->get_optional() + this->get_required()))){
			this->perform(flags, params);
			return true;
		}
		else return false;
	}



	std::string Option::get_help(){ 
		std::string helper = this->help + "\n\nFlags:\n";
		for(int i = 0; i < this->flags.size(); i++)
			helper+=" -"+this->flags.at(i).name +"   - "+ this->flags.at(i).help +"\n";

		helper += + "\nParams:\n";
		for(auto i = this->options.begin(); i != this->options.end(); i++)
			helper+=i->first + "   - "+i->second->help+"\n";
		return helper; 
	}



