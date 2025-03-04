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

void saveTestHistory(string food, float pH, string colour, string result){
    ofstream file("test_history.txt", ios::app);
    file <<"Food: " << food <<" , pH: " << pH << " , Colour: " << colour << " , Result: " << result << endl;
    file.close();
}

void displayTestHistory(){
    ifstream file("test_history.txt");
    if(!file){
        cout << "No previous test history found!" << endl;
        return;
    }
    cout << endl << "Previous Tests: " << endl;
    string line;
    while(getline(file, line)){
        cout << line << endl;
    }
    file.close();
}

void menu(vector<Food>& database){
    int choice;
    do {
        cout<< endl<< " Food Adulteration Detection System " << endl;
        cout <<"press[1] to Check Food for Adulteration " << endl;
        cout <<"press[2] to View Previous Test History " << endl;
        cout <<"press[3] to Exit " << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if(choice == 1){
           string food_name, user_colour;
           float user_pH;

           cout << "Enter food name (e.g turmeric, milk ..): ";
           cin >> food_name;
           cout << "Enter observed pH level: ";
           cin >> user_pH;
           cout << "Enter observed colour: ";
           cin >> user_colour;

           checkAdulteration(database, food_name, user_pH, user_colour);
           saveTestHistory(food_name, user_pH, user_colour, "Checked");
        }else if(choice == 2){
            displayTestHistory();
        }
    }while(choice != 3);
}

int main(){
    vector<Food> data = loadFoodDatabase();
    menu(data);
    return 0;
}