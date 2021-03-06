// command(?)

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <list>

using namespace std;

// on and off states
enum State { off = 0, on = 1 }; 

///////////////// old implementation /////////////////////
/*
//int  MAX = 100;
struct Luz {
    int color;
    int intensidad;

    Luz(){
        intensidad = 0;
    }

    bool toggle(){
        if (intensidad = 0)
            intensidad = 100;
        else
            intensidad = 0;
        return 1;
    }

    bool subir_intensidad() {
        if(intensidad < 100) {
            intensidad += 10;
        }
        else {
            cout <<"La luz ya esta a su maxima intensidad " << endl;
            return 0;
        }
        return 1;
    }
    
    bool bajar_intensidad(){
        if (intensidad > 0){
            intensidad -= 10;
        }
        else{
            cout <<"La luz ya esta a su minima intensidad " << endl;
            return 0;
        }
    }
};

struct Ventilador {
    int velocidad;
    
    Ventilador() {
        velocidad = 0;
    };
    
    bool subir_velocidad() {
        if(velocidad < 10) {
            velocidad += 1;
        }
        else {
            cout <<"El ventilador ya esta a su maxima velocidad " << endl;
            return 0;
        }
        return 1;
    }

    bool bajar_velocidad(){
        if (velocidad > 0){
            velocidad -= 1;
        }
        else{
            cout <<"El ventilador ya esta a su minima velocidad " << endl;
            return 0;
        }
        return 1;
    }
};

class Ambiente{
public:
    map<string, Luz> luces;
    Ventilador v;
    
    Ambiente(){
        v = Ventilador();
    };

    // void PrintState(){
        
    //     cout << "La velocidad del Fan es: " << v.velocidad << "%" << endl;
    //     cout << endl;
    // }

    bool insertar_luz(string color) {
        Luz l = Luz();
        luces.insert(pair<string,Luz>(color, l));
        return 1;
    }
};

template<typename objeto>
class Boton{
private:
    objeto objetivo;
    string descripcion;
    bool (*funcion)();
public:
    void mostrar_descripcion (){
        cout  << '\n' << descripcion;
    }
    Boton(objeto obj, std::string desc, bool(*func)()) : objeto{obj}, descripcion{desc}, funcion{func}{}
    
    bool accionar() {
        return objetivo.funcion();
    }
};

template<typename objeto>
class Control{
public:
    Ambiente target;
    vector<Boton*> botones;
    
    void mostrar_funciones() {
        cout  << "\n\tEscriba el # de la funci??n que quiere utilizar.\n";
        for (auto& boton : botones){
            (boton)->mostrar_descripcion();
        }
    }
};
*/
//////////////////////////////////////////////////////////


///////////////// new implementation /////////////////////

//- COMMAND PATTERN -//

// Receivers:
// class Light (parent: Device)
// class Fan (parent: Device)
class Device{
protected:
    int id;
    State currentState = off;
public:
    State toggle(){
        if (currentState == off)
            currentState = on;
        else
            currentState = off;
        return currentState;
    }
    State getCurrentState(){
        return currentState;
    }
    void printCurrentState(){
        printf("Device State: %s\n", (getCurrentState()) ? "on" : "off" );
    }
    int getId(){
        return id;
    }
};

class Light : public Device{
private:
    string color;
public:
    Light(int _id, string _color): color{_color}{id = _id;};
};

class Fan : public Device{
private:
    int maxSpeed;
    int minSpeed;
    int currentSpeed;

public:
    Fan(int _id){id = _id; minSpeed = 0; maxSpeed = 100; currentSpeed = 0;};
    Fan(int _id, int _minSpeed, int _maxSpeed): minSpeed{_minSpeed}, maxSpeed{_maxSpeed}{id = _id;};

    void printCurrentSpeed(){
        printf("Device Speed: %d%\n", currentSpeed);
    }

    int getCurrentSpeed(){
        return currentSpeed;
    }

    bool increaseSpeed(int amount){
        if ((currentSpeed + amount) < maxSpeed)
            currentSpeed += amount;
        else
            currentSpeed = maxSpeed;

        if (getCurrentState() == off){
            toggle();
        }
        return 1;
    }

    bool decreaseSpeed(int amount){
        if ((currentSpeed - amount) > minSpeed)
            currentSpeed -= amount;
        else
            currentSpeed = minSpeed;

        if (currentSpeed == minSpeed && getCurrentState() == on){
            toggle();
        }
        return 1;
    }
};

// Command parent class
class Command{
public:
    virtual bool execute() const = 0;
};

// Command implementations
class toggleCommand : public Command{
protected:
    Device* d;
public:
    toggleCommand(Device* _d){ d = _d;};

    bool execute() const override{
        d->toggle();
        return true;
    }
};

class increaseCommand : public Command{
protected:
    Fan* d;
    int amount;
public:
    increaseCommand(Fan* _d, int _amount){ d = _d; amount = _amount;};
    
    bool execute() const override{
        d->increaseSpeed(amount);
        return true;
    }
};

class decreaseCommand : public Command{
protected:
    Fan* d;
    int amount;
public:
    decreaseCommand(Fan* _d, int _amount){ d = _d; amount = _amount;};
    
    bool execute() const override{
        d->decreaseSpeed(amount);
        return true;
    }
};

// Invoker
class Button{
private:
    Command* onPress;
public:
    Button(Command* _action){
        onPress = _action;
    }
    bool press(){
        return onPress->execute();
    }
};

// Stores and creates buttons
class Remote{ // should use a ID to identify each button instead of label 
protected:
    map<string, Button*> Layout;
    list<string> Labels;
public:
    string addButton(string label, Button* _button){
        Layout.insert(make_pair(label, _button));
        Labels.push_back(label);
        return label;
    }
    list<string> getLabels(){
        return Labels;
    }

    Button* getButton(string label){
        return Layout[label]; // if not exist, exception and handle.
    }
};


// AMBIENT MUST BECOME A SINGLETON
// Application / Client
class Ambient{
    Remote ambientRemote;
    map<int, Device*> deviceMap;
    int nextID = 0;
public:
    Ambient(){};

    Command* newToggleCommand(Device* device){
        return new toggleCommand(device);
    }

    Command* newIncreaseCommand(Fan* fan, int amount){
        return new increaseCommand(fan, amount);
    }

    Command* newDecreaseCommand(Fan* fan, int amount){
        return new decreaseCommand(fan, amount);
    }

    string createButton(string name, Command* command){
        return ambientRemote.addButton(name, new Button(command));
    }
    
    int createLight(string color){
        int currentID = nextID++;
        Light* light = new Light(currentID, color);
        deviceMap.insert(make_pair(currentID, light));
        return currentID;
    }

    int createFan(){
        int currentID = nextID++;
        Fan* fan = new Fan(currentID);
        deviceMap.insert(make_pair(currentID, fan));
        return currentID;
    }

    int createFan(int minSpeed, int maxSpeed){
        int currentID = nextID++;
        Fan* fan = new Fan(currentID, minSpeed, maxSpeed);
        deviceMap.insert(make_pair(currentID, fan));
        return currentID;
    }

    Device* getDevice(int ID){
        return deviceMap[ID]; // if not exist, exception and handle.
    }

    Light* getLightDevice(int ID){
        return (Light*)deviceMap[ID]; // if not exist, exception and handle.
    }
    Fan* getFanDevice(int ID){
        return (Fan*)deviceMap[ID]; // if not exist, exception and handle.
    }
    
    Button* getButton(string label){
        return ambientRemote.getButton(label); // if not exist, exception and handle.
    }
};

int main(){
    auto a = Ambient();
    auto light1ID = a.createLight("Blue");
    auto light2ID = a.createLight("Red");
    auto fan1ID = a.createFan();

    auto Button1ID = a.createButton("Blue Fan Toggle",  a.newToggleCommand(a.getDevice(light1ID)));
    auto Button2ID = a.createButton("Red Fan Toggle",   a.newToggleCommand(a.getDevice(light2ID)));
    auto Button3ID = a.createButton("Green Fan Toggle", a.newToggleCommand(a.getDevice(a.createLight("Blue"))));

    auto Button4ID = a.createButton("Fan 1 Increase Speed by 10%", a.newIncreaseCommand(a.getFanDevice(fan1ID), 10));
    auto Button5ID = a.createButton("Fan 1 Decrease Speed by 10%", a.newDecreaseCommand(a.getFanDevice(fan1ID), 10));

    // Simple test
  
    printf("\n turning blue light on and off\n");
    a.getDevice(light1ID)->printCurrentState();
    printf("\n");

    a.getButton(Button1ID)->press();
    
    a.getDevice(light1ID)->printCurrentState();
    printf("\n");
    
    a.getButton(Button1ID)->press();
    
    a.getDevice(light1ID)->printCurrentState();
    printf("\n");
    
    printf("\n increasing fan speed\n");
    a.getFanDevice(fan1ID)->printCurrentState(); 
    a.getFanDevice(fan1ID)->printCurrentSpeed();
    printf("\n");

    a.getButton(Button4ID)->press();

    a.getFanDevice(fan1ID)->printCurrentState(); 
    a.getFanDevice(fan1ID)->printCurrentSpeed();
    printf("\n");
    
    a.getButton(Button4ID)->press();
    
    a.getFanDevice(fan1ID)->printCurrentState(); 
    a.getFanDevice(fan1ID)->printCurrentSpeed();
    printf("\n");

    for (auto i = 0; i <= 10; ++i )
        a.getButton(Button4ID)->press();

    a.getFanDevice(fan1ID)->printCurrentState(); 
    a.getFanDevice(fan1ID)->printCurrentSpeed();
    printf("\n");
    
    printf("\n decreasing fan speed\n");
    a.getButton(Button5ID)->press();

    a.getFanDevice(fan1ID)->printCurrentState(); 
    a.getFanDevice(fan1ID)->printCurrentSpeed();
    printf("\n");

    a.getButton(Button5ID)->press();

    a.getFanDevice(fan1ID)->printCurrentState(); 
    a.getFanDevice(fan1ID)->printCurrentSpeed();
    printf("\n");
    
    for (auto i = 0; i <= 10; ++i )
        a.getButton(Button5ID)->press();

    a.getFanDevice(fan1ID)->printCurrentState(); 
    a.getFanDevice(fan1ID)->printCurrentSpeed();
    printf("\n");
}

// TODO: Ambient should become a singleton
/*
int main() {
    auto a = Ambiente();
    auto c = Control();
    
    a.insertar_luz("Yellow");
    cout << a.luces["Yellow"].intensidad << endl;

    auto luz = Luz();
    auto boton1 = new Boton("Luz Amarilla", luz, &luz.toggle);
    
    c.botones.push_back(new Boton("Luz Amarilla", &a::luces["Yellow"]::toggle() );
}
*/




