#include "symbol_info.h"

class scope_table
{ 
private:
    int bucket_count;
    int unique_id;
    scope_table *parent_scope = NULL;
    vector<list<symbol_info *>> table;
    //one hash function
    int hash_function(string name)
{
    int hash = 0;
    for (char ch : name)
    {
        hash += static_cast<int>(ch); 
    }
    return hash % bucket_count; 
}



public:
//
    scope_table() : bucket_count(101), unique_id(0), parent_scope(nullptr), table(vector<list<symbol_info*>>(101)) {}
    scope_table(int bucket_count, int unique_id, scope_table *parent_scope);
    scope_table *get_parent_scope();
    int get_unique_id();
    symbol_info *lookup_in_scope(symbol_info* symbol);
    bool insert_in_scope(symbol_info* symbol);
    bool delete_from_scope(symbol_info* symbol);
    void print_scope_table(ofstream& outlog, int current_scope_id);
    ~scope_table();

    
};


scope_table::scope_table(int bucket_count, int unique_id, scope_table *parent_scope) {
    this->bucket_count = bucket_count;
    this->unique_id = unique_id;
    this->parent_scope = parent_scope;

    this->table = vector<list<symbol_info*>>(bucket_count);
}

scope_table* scope_table::get_parent_scope() {
    return parent_scope;
}

int scope_table::get_unique_id() {
    return unique_id;
}


symbol_info* scope_table::lookup_in_scope(symbol_info* symbol) {
    int index = hash_function(symbol->get_name());
  
    for (symbol_info* sym : table[index]) {
        if (sym->get_name() == symbol->get_name()) {
            return sym;
        }
    }
    return nullptr;
}

bool scope_table::insert_in_scope(symbol_info* symbol) {
    if (lookup_in_scope(symbol) != nullptr) {
        return false; 
    }
    int index = hash_function(symbol->get_name());
 
    table[index].push_back(symbol);
    return true;
}






void scope_table::print_scope_table(ofstream& outlog, int current_scope_id) {
    

    outlog << "ScopeTable # " << current_scope_id << endl;
 
    for (int i = 0; i < bucket_count; ++i) {
        
        if (!table[i].empty()) {
            
            outlog << i << " --> ";
           
            for (auto symbol : table[i]) {
                
                std::string name = symbol->get_name();

                 
                if (name.find('[') != std::string::npos && name.find(']') != std::string::npos) {
                    continue;
                }
                
                outlog << "< " << symbol->get_name() << " : " << "ID" << " >" << endl;

                
                if (!symbol->get_param_types().empty() && !symbol->get_param_names().empty()) {
                    
                    outlog << "Function Definition" << endl;
                    outlog << "Return Type: " << symbol->get_type() << endl;
                    outlog << "Number of Parameters: " << symbol->get_param_types().size() << endl;

                   
                    outlog << "Parameter Details: ";
                    
                    for (size_t j = 0; j < symbol->get_param_types().size(); ++j) {
                        outlog << symbol->get_param_types()[j] << " " << symbol->get_param_names()[j];
                        
                        if (j != symbol->get_param_types().size() - 1) {
                            outlog << ", ";  
                        }
                    }
                    outlog << endl;
                } else if (symbol->get_array_size() > 0) {
                    
                    outlog << "Array" << endl;
                    outlog << "Type: " << symbol->get_type() << endl;
                    outlog << "Size: " << symbol->get_array_size() << endl;
                }
                else {
                    
                    
                    outlog << "" << symbol->get_data_type() << endl;
                    outlog << "Type: " << symbol->get_type() << endl;
                }

                outlog << endl; 
            }
        }
    }

    outlog << "---------" << endl;
}




scope_table::~scope_table()
{
    for (int i = 0; i < bucket_count; ++i)
    {
        for (auto symbol : table[i])
        {
            delete symbol;
        }
    }
    
}













//--------------------
// bool scope_table::delete_from_scope(symbol_info* symbol) { // this will bascially return false in case the symbol isn't found 
//     int index = hash_function(symbol->get_name());
//     // auto& symbols means that it is a reference to the list of symbol_info pointers
//     auto& symbols = table[index];
//     //iterates over start to end of the list of symbol_info pointers
//     for (auto it = symbols.begin(); it != symbols.end(); ++it) {
//         //if the name matches
//         if ((*it)->get_name() == symbol->get_name()) {
//             //delete the symbol and then erase it from the list
//             // * it is the symbol_info pointer delete will delete the pointer
//             delete *it;
//             symbols.erase(it);
//             return true;
//         }
//     }
//     return false; 
// }