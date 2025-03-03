#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<map>

using namespace std;

struct Food{
    string name;
    float normal_pH_low, normal_pH_high;
    string normal_colour;
    vector<string> adulterants;
    vector<string> health_effects;
    vector<string> tests;
};

vector<Food> loadFoodDatabase(){
    vector<Food> database;
    ifstream file("food_data.txt");
    if(!file){
        cout << "Error: Database file not found!" << endl;
        return database;
    }

    string line;
    while(getline(file, line)){
        stringstream ss(line);
        Food temp;
        string adulterant, health_effect, test;
        ss >> temp.name >> temp.normal_pH_low >> temp.normal_pH_high >> temp.normal_colour;
        while(ss >> adulterant >> health_effect >> test){
            temp.adulterants.push_back(adulterant);
            temp.health_effects.push_back(health_effect);
            temp.tests.push_back(test);
        }
        database.push_back(temp);
    }
    file.close();
    return database;
}

void checkAdulteration(vector<Food>& database, string food_name, float user_pH, string user_colour){
    for(const auto& food: database){
        if(food.name == food_name){
            bool isAdulterated = false;
            cout << "\n Checking.." << food_name << " for adulteration.. \n" ;
            if (user_pH < food.normal_pH_low || user_pH > food.normal_pH_high){
                cout << "Abnormal ph detected! " << endl;
                isAdulterated = true;
            }
            if(user_colour != food.normal_colour){
                cout << "Abnormal colour detected! " << endl;
                isAdulterated = true;
            }
            if(isAdulterated){
                cout << "\n possible adulterants & effects: " << endl; 
                for(size_t i = 0; i < food.adulterants.size() ; i++){
                    cout << " Adulterant: " << food.adulterants[i] <<"| effects: " << food.health_effects[i] << endl;
                    cout << "DIY Test: " << food.tests[i] << endl;
                }
            }else{
                cout << "No adulteration detected. "<< endl;
            }
            return;
        }
    }
    cout << "Food item not found in database.  " << endl;
}

int main(){
    vector<Food> data = loadFoodDatabase();
    for(Food val: data){
        cout << val.name << endl;
        cout << val.normal_colour << endl;
        cout << val.normal_pH_high << endl;
        cout << val.normal_pH_low << endl << endl;
    }
    return 0;
}