#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Supporting functions

int ManhattanDistance(const Position &pos1, const Position &pos2)
{
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
};


// Definition of classes    

// Map part

const Position Position::npos(-1,-1);

Position::Position(int r, int c)
{
    this->r = r;
    this->c = c;
}

Position::Position(const string &str_pos)
{
    int pos = str_pos.find(',');
    r = stoi(str_pos.substr(1,pos-1));
    c = stoi(str_pos.substr(pos+1,str_pos.size()-3));    
}

int Position::getRow() const
{
    return r;
}

int Position::getCol() const
{
    return c;
}

void Position::setRow(int r)
{
    this->r = r;
}

void Position::setCol(int c)
{
    this->c = c;
}

string Position::str() const
{
    return "(" + to_string(r) + "," + to_string(c) + ")";
}

bool Position::isEqual(const Position & pos) const
{
    return (pos.getRow() == r) && (pos.getCol() == c);
}

MapElement::MapElement(ElementType in_type)
{
    type = in_type;
}

MapElement::~MapElement() {}

FakeWall::FakeWall(int in_req_exp) 
    : MapElement(FAKE_WALL)
{
    req_exp = in_req_exp;
}

Path::Path() 
    : MapElement(PATH) 
{}

Wall::Wall() 
    : MapElement(WALL) 
{}

int FakeWall::getReqExp() const
{
    return req_exp;
}

int Map::getNumCols() const
{
    return num_cols;
}

int Map::getNumRows() const
{
    return num_rows;
}

Map::Map(int num_rows, int num_cols, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
    : num_rows(num_rows), num_cols(num_cols)
{  
    map = new MapElement**[num_rows];
    for (int i = 0; i < num_rows; i++) {
        map[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; ++j)
        {
            map[i][j] = new Path();
        }
    }

    for (int i = 0; i < num_fake_walls; i++) {
        int r = array_fake_walls[i].getRow();
        int c = array_fake_walls[i].getCol();
        delete map[r][c];
        map[r][c] = new FakeWall((r * 257 + c * 139 + 89) % 900 + 1);
    }

    for (int i = 0; i < num_walls; i++) {
        int r = array_walls[i].getRow();
        int c = array_walls[i].getCol();
        delete map[r][c];
        map[r][c] = new Wall();
    }
}

Map::~Map()
{
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}

ElementType MapElement::getType() const
{
    return type;
}

ElementType Map::getElementType(const Position &pos) const
{
    return (map[pos.getRow()][pos.getCol()])->getType();
}

ElementType Map::getElementType(int in_r, int in_c) const
{
    return (map[in_r][in_c])->getType();
}

bool Map::isValid(const Position &pos, MovingObject* mv_obj) const
{
    int r = pos.getRow();
    int c = pos.getCol();
    if (r < 0 || r >= num_rows || c < 0 || c >= num_cols) return false;
    MapElement* map_element = map[pos.getRow()][pos.getCol()];
    if (map_element->getType() == PATH) return true;
    if (map_element->getType() == WALL) return false;
    if (mv_obj->getObjectType() == WATSON) {
        if (map_element->getType() == PATH) return true;
        else if (map_element->getType() == FAKE_WALL) {
            Watson* watson = dynamic_cast<Watson*> (mv_obj);
            if(watson->getEXP() > ((r*257 + c*139 + 89)% 900 + 1)) return true;
            else return false;
        }
        else return false;
    }
    return true;
}


// Array Moving Object part

// Moving Object

MovingObject::MovingObject(int index, const Position pos, Map * map, const string & name)
{
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
}

MovingObject::~MovingObject()
{}

Position MovingObject::getCurrentPosition() const
{
    return pos;
}

// Class Character

Character::Character(int index, const Position pos, Map *map, const string &name)
    : MovingObject(index, pos, map, name) {}

void Character::action(ArrayMovingObject* arr_mv_objs, SherlockBag*  sherlockbag, WatsonBag* watsonbag)
{}
// Class Sherlock

Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Sherlock"), moving_rule(moving_rule)
{
    if (init_hp > 500) hp = 500;
    else if (init_hp < 0) hp = 0;
    else hp = init_hp;

    if (init_exp > 900) exp = 900;
    else if (init_exp < 0) exp = 0;
    else exp = init_exp;
}

void Sherlock::setHP(int init_hp) 
{
    if (init_hp > 500) hp = 500;
    else if (init_hp < 0) hp = 0;
    else hp = init_hp;
}

void Sherlock::setEXP(int init_exp) 
{
    if (init_exp < 0) exp = 0;
    else if (init_exp > 900) exp = 900;
    else exp = init_exp;
}

int Sherlock::getEXP() const
{
    return exp;
}

int Sherlock::getHP() const
{
    return hp;
}

MovingObjectType Sherlock::getObjectType() const
{
    return SHERLOCK;
}

Position Sherlock::getNextPosition() 
{
    char dir = moving_rule[move_count++ % moving_rule.length()];
    Position next_position = pos;
    switch(dir){
        case 'L': next_position.setCol(next_position.getCol()-1);
            break;
        case 'R': next_position.setCol(next_position.getCol()+1);
            break;
        case 'U': next_position.setRow(next_position.getRow()-1);
            break;
        case 'D': next_position.setRow(next_position.getRow()+1);
            break; 
        default:
            break;
    }
    if (map->isValid(next_position, this)) return next_position;
    else return Position::npos;
    return Position::npos;
}

void Sherlock::move()
{
    Position next_position = getNextPosition();
    if(next_position.isEqual(Position::npos)) return;
    this->pos = next_position;
}

string Sherlock::str() const
{
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";  
}

void Sherlock::action(ArrayMovingObject* arr_mv_objs, SherlockBag*  sherlockbag, WatsonBag* watsonbag)
{
    for (int i = 0; i < arr_mv_objs->size(); i++) 
    {
        MovingObject* mv_obj = arr_mv_objs->get(i);
        if (mv_obj->getCurrentPosition().isEqual(this->getCurrentPosition())) 
        {
            MovingObjectType type = mv_obj->getObjectType();
            if (type == WATSON) 
            {
                BaseItem* temp_sherlock_item = sherlockbag->get(PASSING_CARD);
                BaseItem* temp_watson_item = watsonbag->get(EXCEMPTION_CARD);
                if (temp_sherlock_item != nullptr && temp_watson_item != nullptr)
                {
                    while (temp_sherlock_item != nullptr && temp_watson_item != nullptr)
                    {
                        watsonbag->insert(temp_sherlock_item);
                        sherlockbag->insert(temp_watson_item);
                        temp_sherlock_item = sherlockbag->get(PASSING_CARD);
                        temp_watson_item = watsonbag->get(EXCEMPTION_CARD);
                    }
                }

            }
            if (type == ROBOT) 
            {
                Robot* robot = dynamic_cast<Robot*> (mv_obj);
                RobotType robot_type = robot->getType();
                BaseItem* item = sherlockbag->get();
                if (robot_type == S) 
                {
                    if (exp > 400) 
                    {
                        sherlockbag->insert(robot->getItem());
                        delete robot;
                    }
                    else 
                    {
                        BaseItem* item = sherlockbag->get(EXCEMPTION_CARD);
                        if (item != nullptr) 
                        {
                            ExcemptionCard* excemption_card = dynamic_cast <ExcemptionCard* > (item);
                            if (!excemption_card->canUse(this,robot)) exp = exp*0.9;
                        }
                    }
                }
                else if (robot_type == W) 
                {
                    sherlockbag->insert(robot->getItem());
                    delete robot;
                }
                else if (robot_type == SW) 
                {
                    if (exp > 300 && hp > 335) {
                        sherlockbag->insert(robot->getItem());
                        delete robot;
                    }
                    else {
                        if (item != nullptr) {
                            ExcemptionCard* excemption_card = dynamic_cast <ExcemptionCard* > (item);
                            if (!excemption_card->canUse(this,robot)) {
                                exp *= 0.85;
                                hp *= 0.85;
                            }
                        }
                    }
                }
                else if (robot_type == C) 
                {
                    if (exp > 500) 
                    {
                        // sherlock pos = criminal pos
                        this->pos = robot->getNextPosition();
                    }
                    else 
                    {
                        sherlockbag->insert(robot->getItem());
                        delete robot;
                    }
                }

                // Use item after encountering a robot
                BaseItem* afterItem = sherlockbag->get();
                afterItem->use(this,nullptr);
            }
        }
    }
}

// Watson

Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Watson"), moving_rule(moving_rule)
{
    if (init_hp > 500) hp = 500;
    else if (init_hp < 0) hp = 0;
    else hp = init_hp;

    if (init_exp > 900) exp = 900;
    else if (init_exp < 0) exp = 0;
    else exp = init_exp;
}

MovingObjectType Watson::getObjectType() const
{
    return WATSON;
}

Position Watson::getNextPosition() {
    char dir = moving_rule[move_count++ % moving_rule.size()];
    Position next_position = pos;
    switch(dir){
        case 'L': next_position.setCol(next_position.getCol()-1);
            break;
        case 'R': next_position.setCol(next_position.getCol()+1);
            break;
        case 'U': next_position.setRow(next_position.getRow()-1);
            break;
        case 'D': next_position.setRow(next_position.getRow()+1);
            break; 
    }
    return map->isValid(next_position, this) ? next_position : Position::npos;
}

void Watson::move()
{
    Position next_position = getNextPosition();
    if(next_position.isEqual(Position::npos)) return;
    pos = next_position;
}

string Watson::str() const
{
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";  
}

int Watson::getEXP() const
{
    return exp;
}

int Watson::getHP() const
{
    return hp;
}

void Watson::setEXP(int init_exp) 
{
    if (init_exp < 0) exp = 0;
    else if (init_exp > 900) exp = 900;
    else exp = init_exp;
}

void Watson::setHP(int init_hp) 
{
    if (init_hp > 500) hp = 500;
    else if (init_hp < 0) hp = 0;
    else hp = init_hp;
}

void Watson::action(ArrayMovingObject* arr_mv_objs, SherlockBag*  sherlockbag, WatsonBag* watsonbag)
{
    for (int i = 0; i < arr_mv_objs->size(); i++) 
    {
        MovingObject* mv_obj = arr_mv_objs->get(i);
        if (mv_obj->getCurrentPosition().isEqual(this->getCurrentPosition())) 
        {
            MovingObjectType type = mv_obj->getObjectType();
            if (type == WATSON) 
            {
                BaseItem* temp_sherlock_item = sherlockbag->get(PASSING_CARD);
                BaseItem* temp_watson_item = watsonbag->get(EXCEMPTION_CARD);
                if (temp_sherlock_item != nullptr && temp_watson_item != nullptr)
                {
                    while (temp_sherlock_item != nullptr && temp_watson_item != nullptr)
                    {
                        watsonbag->insert(temp_sherlock_item);
                        sherlockbag->insert(temp_watson_item);
                        temp_sherlock_item = sherlockbag->get(PASSING_CARD);
                        temp_watson_item = watsonbag->get(EXCEMPTION_CARD);
                    }
                }

            }
            if (type == ROBOT) 
            {
                Robot* robot = dynamic_cast<Robot*> (mv_obj);
                RobotType robot_type = robot->getType();
                BaseItem* item = sherlockbag->get();
                if (robot_type == S) 
                {
                    if (exp > 400) 
                    {
                        sherlockbag->insert(robot->getItem());
                        delete robot;
                    }
                    else 
                    {
                        BaseItem* item = sherlockbag->get(EXCEMPTION_CARD);
                        if (item != nullptr) 
                        {
                            ExcemptionCard* excemption_card = dynamic_cast <ExcemptionCard* > (item);
                            if (!excemption_card->canUse(this,robot)) exp = exp*0.9;
                        }
                    }
                }
                else if (robot_type == W) 
                {
                    sherlockbag->insert(robot->getItem());
                    delete robot;
                }
                else if (robot_type == SW) 
                {
                    if (exp > 300 && hp > 335) {
                        sherlockbag->insert(robot->getItem());
                        delete robot;
                    }
                    else {
                        if (item != nullptr) {
                            ExcemptionCard* excemption_card = dynamic_cast <ExcemptionCard* > (item);
                            if (!excemption_card->canUse(this,robot)) {
                                exp *= 0.85;
                                hp *= 0.85;
                            }
                        }
                    }
                }
                else if (robot_type == C) 
                {
                    if (exp > 500) 
                    {
                        // sherlock pos = criminal pos
                        this->pos = robot->getNextPosition();
                    }
                    else 
                    {
                        sherlockbag->insert(robot->getItem());
                        delete robot;
                    }
                }

                // Use item after encountering a robot
                BaseItem* afterItem = sherlockbag->get();
                if (afterItem != nullptr) afterItem->use(this,nullptr);
            }
        }
    }
}

// Criminal

Criminal::Criminal(int index, const Position & init_pos, Map* map, Sherlock* sherlock, Watson* watson)
    : Character(index, init_pos, map, "Criminal"), sherlock(sherlock), watson(watson)
{
}

Position Criminal::getNextPosition()
{
    char dir;
    int distance_U = INT_MIN , distance_D = INT_MIN , distance_L = INT_MIN , distance_R = INT_MIN ;
    Position U = pos, D = pos, L = pos, R = pos;
    U.setRow(pos.getRow()-1);
    D.setRow(pos.getRow()+1);
    L.setCol(pos.getCol()-1);
    R.setCol(pos.getCol()+1);

    if (map->isValid(U,this)) distance_U = ManhattanDistance(U,watson->getCurrentPosition()) + ManhattanDistance(U,sherlock->getCurrentPosition());

    if (map->isValid(D,this)) distance_D = ManhattanDistance(D,watson->getCurrentPosition()) + ManhattanDistance(D,sherlock->getCurrentPosition());

    if (map->isValid(L,this)) distance_L = ManhattanDistance(L,watson->getCurrentPosition()) + ManhattanDistance(L,sherlock->getCurrentPosition());

    if (map->isValid(R,this)) distance_R = ManhattanDistance(R,watson->getCurrentPosition()) + ManhattanDistance(R,sherlock->getCurrentPosition());

    
    int temp = max(max(distance_U,distance_D),max(distance_L,distance_R));
    if (temp == distance_U) {
        if (map->isValid(U,this)) {
            move_count++;
            pos = U;
        }
        return pos;
    }
    else if (temp == distance_L) {
        if (map->isValid(L,this)) {
            move_count++;
            pos = L;
        }
        return pos;
    }
    else if (temp == distance_D) {
        if (map->isValid(D,this)) {
            move_count++;
            pos = D;
        }
        return pos;
    }
    else if (temp == distance_R) {
        if (map->isValid(R,this)) {
            move_count++;
            pos = R;
        }
        return pos;
    }
    return Position::npos;
}


void Criminal::move()
{
    Position next_position = getNextPosition();
    if(next_position.isEqual(Position::npos)) return;
    pos = next_position;
}

string Criminal::str() const
{
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}

int Criminal::getCount() const
{
    return move_count;
}

bool Criminal::isCreatedRobotNext() const
{
    if(getCount()%2 == 0 && getCount() != 0 && getCount() != 2) return true;
    return false;
}

int Criminal::getEXP() const
{
    return 0;
}

int Criminal::getHP() const
{
    return 0;
}

void Criminal::setEXP(int init_exp) 
{
}

void Criminal::setHP(int init_hp) 
{
}

MovingObjectType Criminal::getObjectType() const
{
    return CRIMINAL;
}

void Criminal::create(ArrayMovingObject* arr_mv_objs, Sherlock* sherlock, Watson* watson)
{
    if (arr_mv_objs->isFull()) return;
    Robot* robot = nullptr;
    if (getCount() == 2) {
        robot = new RobotC(arr_mv_objs->size(), getCurrentPosition(), map, this);
    }

    if (isCreatedRobotNext()) {
        int criminal_sherlock_distance = ManhattanDistance(getCurrentPosition(), sherlock->getCurrentPosition());
        int criminal_watson_distance =  ManhattanDistance(getCurrentPosition(), watson->getCurrentPosition());
        int temp = min(criminal_sherlock_distance,criminal_watson_distance);

        if (temp == criminal_sherlock_distance && temp == criminal_watson_distance) {
            robot = new RobotSW(arr_mv_objs->size(), getCurrentPosition(), map, this, sherlock, watson);
        }
        else if (temp == criminal_sherlock_distance) {
            robot = new RobotS(arr_mv_objs->size(), getCurrentPosition(), map, this, sherlock);
        }
        else {
            robot = new RobotW(arr_mv_objs->size(), getCurrentPosition(), map, this, watson);
        }
    }
    arr_mv_objs->add(robot);
    delete robot;
}

// Array Moving Object

ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    arr_mv_objs = new MovingObject*[capacity]();
    /*for (int i = 0; i < capacity; i++) {
        arr_mv_objs[i] = nullptr;
    }*/

}

ArrayMovingObject::~ArrayMovingObject()
{
    delete[] arr_mv_objs;
}
 
bool ArrayMovingObject::isFull() const
{
    return count >= capacity;
}

int MovingObject::getIndex() const
{
    return index;
}

bool ArrayMovingObject::add(MovingObject* mv_obj)
{
    if (isFull()) return false;
    arr_mv_objs[mv_obj->getIndex()] = mv_obj;
    count++;
    return true;
}

MovingObject* ArrayMovingObject::get(int index) const
{
    return arr_mv_objs[index];
}

string ArrayMovingObject::str() const
{
    string res = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity);
    for(int i = 0; i < capacity; i++) {
        if (arr_mv_objs[i]) res += ";" + arr_mv_objs[i]->str();
    }
    res += "]";
    return res;
}

int ArrayMovingObject::MovingObjectCount() const
{
    return count;
}

int ArrayMovingObject::size() const
{
    return capacity;
}


// Config part

/*Configuration::Configuration(const string & filepath)
{
    // TODO:
    ifstream inputFile;
    inputFile.open(filepath);
    if (inputFile.fail()) {
        cout << "Error opening the file\n";
    }
    else {
        string line;
        while(getline(inputFile,line)) {
            string key = line.substr(0,line.find('='));
            string value = line.substr(line.find('=')+1,line.size());
            if (key == "MAP_NUM_ROWS") {
                map_num_rows = stoi(value);
            } 
            else if (key == "MAP_NUM_COLS") {
                map_num_cols = stoi(value);
            } 
            else if (key == "MAX_NUM_MOVING_OBJECTS") {
                max_num_moving_objects = stoi(value);
            }
            else if (key == "ARRAY_WALLS") {
                size_t pos1=0,pos2=0;
                while ((pos1 = line.find(',', pos1)) != string::npos) {
                    num_walls++;
                    pos1++;
                }
                arr_walls = new Position[num_walls];
                for (int i = 0; i < num_walls; i++){
                    size_t open_pos = line.find('(', pos2);
                    size_t comma_pos = line.find(',', open_pos);
                    size_t close_pos = line.find(')', comma_pos);
                    string row = line.substr(open_pos + 1, comma_pos - open_pos - 1);
                    string col = line.substr(comma_pos + 1, close_pos - comma_pos - 1);
                    arr_walls[i].setRow(stoi(row));
                    arr_walls[i].setCol(stoi(col));
                    pos2 = close_pos + 1;
                }
            } 
            else if (key == "ARRAY_FAKE_WALLS") {
                size_t pos1=0,pos2=0;
                while ((pos1 = line.find(',', pos1)) != string::npos) {
                    num_fake_walls++;
                    pos1++;
                }
                arr_fake_walls = new Position[num_fake_walls];
                for (int i = 0; i < num_fake_walls; i++){
                    size_t open_pos = line.find('(', pos2);
                    size_t comma_pos = line.find(',', open_pos);
                    size_t close_pos = line.find(')', comma_pos);
                    string row = line.substr(open_pos + 1, comma_pos - open_pos - 1);
                    string col = line.substr(comma_pos + 1, close_pos - comma_pos - 1);
                    arr_fake_walls[i].setRow(stoi(row));
                    arr_fake_walls[i].setCol(stoi(col));
                    pos2 = close_pos + 1;
                }
            } 
            else if (key == "SHERLOCK_MOVING_RULE") {
                sherlock_moving_rule = value;
            } 
            else if (key == "SHERLOCK_INIT_POS") {
                sherlock_init_pos = Position(value);  
            } 
            else if (key == "SHERLOCK_INIT_HP") {
                sherlock_init_hp = stoi(value);
            } 
            else if (key == "SHERLOCK_INIT_EXP") {
                sherlock_init_exp = stoi(value);
            }
            else if (key == "WATSON_MOVING_RULE") {
                watson_moving_rule = value;
            }
            else if (key == "WATSON_INIT_POS") {
                watson_init_pos = Position(value); 
            }
            else if (key == "WATSON_INIT_HP") {
                watson_init_hp = stoi(value);
            }
            else if (key == "WATSON_INIT_EXP") {
                watson_init_exp = stoi(value);
            }
            else if (key == "CRIMINAL_INIT_POS") {
                criminal_init_pos = Position(value); 
            }
            else if (key == "NUM_STEPS") {
                num_steps = stoi(value);
            }
        }
    }
    inputFile.close();
}*/


Configuration::Configuration(const string & filepath)
{
    ifstream inputFile;
    inputFile.open(filepath);
    if (inputFile.fail()) {
        cout << "Error opening the file\n";
    }
    else {
        string line;
        while(getline(inputFile,line)) {
            string key = line.substr(0,line.find('='));
            string value = line.substr(line.find('=')+1);
            if (key == "MAP_NUM_ROWS") {
                stringstream ss(value);
                string sth;
                getline(ss,sth,'=');
                ss >> map_num_rows;
            } 
            else if (key == "MAP_NUM_COLS") {
                stringstream ss(value);
                string sth;
                getline(ss,sth,'=');
                ss >> map_num_cols;
            } 
            else if (key == "MAX_NUM_MOVING_OBJECTS") {
                stringstream ss(value);
                string sth;
                getline(ss,sth,'=');
                ss >> max_num_moving_objects;
            }
            else if (key == "ARRAY_WALLS") {
                for (char x : value) {
                    if (x == ';') num_walls++;
                }
                num_walls++;
                arr_walls = new Position[num_walls];
                value = value.substr(1,value.size()-2);
                stringstream ss(value);
                //string item;

                char ch1;
                int a = 0;
                while (ss >> ch1) {
                    if (ch1 == '(') {
                        int num1, num2;
                        ss >> num1;
                        ss.ignore();
                        ss >> num2;
                        arr_walls[a] = Position(num1,num2);
                        a++;
                    }
                }
                /*for (int i = 0; i < num_walls; i++) {
                    getline(ss,item, ';');
                    arr_walls[i] = Position(item);
                }*/
            } 
            else if (key == "ARRAY_FAKE_WALLS") {
                for (char x : value) {
                    if (x == ';') num_fake_walls++;
                }
                num_fake_walls++;
                arr_fake_walls = new Position[num_fake_walls];
                value = value.substr(1,value.size()-2);
                stringstream ss(value);
                //string item;
                char ch1;
                int a = 0;
                while (ss >> ch1) {
                    if (ch1 == '(') {
                        int num1, num2;
                        ss >> num1;
                        ss.ignore();
                        ss >> num2;
                        arr_fake_walls[a] = Position(num1,num2);
                        a++;
                    }
                }
                /*for (int i = 0; i < num_fake_walls; i++) {
                    getline(ss,item, ';');
                    arr_fake_walls[i] = Position(item);
                }*/
            } 
            else if (key == "SHERLOCK_MOVING_RULE") {
                sherlock_moving_rule = value;
            } 
            else if (key == "SHERLOCK_INIT_POS") {
                sherlock_init_pos = Position(value);  
            } 
            else if (key == "SHERLOCK_INIT_HP") {
                stringstream ss(value);
                string sth;
                getline(ss,sth,'=');
                ss >> sherlock_init_hp;
            } 
            else if (key == "SHERLOCK_INIT_EXP") {
                stringstream ss(value);
                string sth;
                getline(ss,sth,'=');
                ss >> sherlock_init_exp;
            }
            else if (key == "WATSON_MOVING_RULE") {
                watson_moving_rule = value;
            }
            else if (key == "WATSON_INIT_POS") {
                watson_init_pos = Position(value); 
            }
            else if (key == "WATSON_INIT_HP") {
                stringstream ss(value);
                string sth;
                getline(ss,sth,'=');
                ss >> watson_init_hp;
            }
            else if (key == "WATSON_INIT_EXP") {
                stringstream ss(value);
                string sth;
                getline(ss,sth,'=');
                ss >> watson_init_exp;
            }
            else if (key == "CRIMINAL_INIT_POS") {
                criminal_init_pos = Position(value); 
            }
            else if (key == "NUM_STEPS") {
                stringstream ss(value);
                string sth;
                getline(ss,sth,'=');
                ss >> num_steps;
            }
        }
    }
    inputFile.close();
}



Configuration::~Configuration()
{
    delete[] this->arr_walls;
    delete[] this->arr_fake_walls;
}

string Configuration::str() const
{
    string arr_walls_string = "[";
    for (int i = 0; i < num_walls; i++) {
        arr_walls_string += arr_walls[i].str();
        if (i != num_walls - 1) arr_walls_string += ";";
    }
    arr_walls_string += "]";

    string arr_fake_walls_string = "[";
    for (int i = 0; i < num_fake_walls; i++) {
        arr_fake_walls_string += arr_fake_walls[i].str();
        if (i != num_fake_walls - 1) arr_fake_walls_string += ";";
    }
    arr_fake_walls_string += "]";

    return "Configuration[\nMAP_NUM_ROWS=" + to_string(map_num_rows) + "\nMAP_NUM_COLS=" + to_string(map_num_cols)
    + "\nMAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\nNUM_WALLS=" + to_string(num_walls)
    + "\nARRAY_WALLS=" + arr_walls_string + "\nNUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\nARRAY_FAKE_WALLS="
    + arr_fake_walls_string + "\nSHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\nSHERLOCK_INIT_POS="
    + sherlock_init_pos.str() + "\nSHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\nSHERLOCK_INIT_EXP="
    + to_string(sherlock_init_exp) + "\nWATSON_MOVING_RULE=" + watson_moving_rule + "\nWATSON_INIT_POS="
    + watson_init_pos.str() + "\nWATSON_INIT_HP=" + to_string(watson_init_hp) + "\nWATSON_INIT_EXP="
    + to_string(watson_init_exp) + "\nCRIMINAL_INIT_POS=" + criminal_init_pos.str()
    + "\nNUM_STEPS=" + to_string(num_steps) + "\n]";
}


// Class Robot

Robot::Robot(int index, const Position &pos, Map* map, RobotType robot_type, Criminal* criminal, const string &name)
    : MovingObject(index, pos, map, "Robot")
{
    this->robot_type = robot_type;
    this->criminal = criminal;

    int p = pos.getRow() * pos.getCol();
    if (p == 0) p = 0;
    else if (p%9 == 0) p = 9;
    else p = p%9;

    switch(p) {
    case 0:
    case 1:
        item = new MagicBook();
        break;
    case 2:
    case 3:
        item = new EnergyDrink();
        break;
    case 4:
    case 5:
        item = new FirstAid();
        break;
    case 6:
    case 7:
        item = new ExcemptionCard();
        break;
    case 8:
    case 9: 
    {
        int t = (pos.getRow()*11 + pos.getCol()) % 4;
        if (t == 0) item = new PassingCard("RobotS");
        else if (t == 1) item = new PassingCard("RobotC");
        else if (t == 2) item = new PassingCard("RobotSW");
        else if (t == 3) item = new PassingCard("all");
        break;
    }
    default:
        break;
    }
}

Robot::~Robot()
{
    delete item;
}

MovingObjectType Robot::getObjectType() const
{
    return ROBOT;
}

BaseItem* Robot::getItem() const
{
    return item;
}

// RobotC

RobotC::RobotC(int index, const Position &init_pos, Map* map, Criminal* criminal)
    : Robot(index, init_pos, map, robot_type, criminal, "RobotC")
{}

Position RobotC::getNextPosition()
{
    Position next_position = pos;
    next_position = criminal->getCurrentPosition();
    if (map->isValid(next_position, this)) return next_position;
    return Position::npos;
}

int RobotC::getDistance(Sherlock* sherlock)
{
    return ManhattanDistance(pos,sherlock->getCurrentPosition());
}

int RobotC::getDistance(Watson* watson)
{
    return ManhattanDistance(pos,watson->getCurrentPosition());
}

int RobotC::getDistance() const
{
    return 0;
}

void RobotC::move()
{   
    Position next_position = getNextPosition();
    if (next_position.isEqual(Position::npos)) return;
    pos = next_position;
}

string RobotC::str() const
{
    return "Robot[pos=" + pos.str() + ";type=C;dist=" + "" + "]";
}

RobotType RobotC::getType() const
{
    return C;
}


// RobotW

RobotW::RobotW(int index, const Position &init_pos, Map* map, Criminal* criminal, Watson* watson)
    : Robot(index, init_pos, map, W, criminal, "RobotW")
{
    this->watson = watson;
}

Position RobotW::getNextPosition()
{
    char dir;
    int distance_U = INT_MAX , distance_D = INT_MAX , distance_L = INT_MAX , distance_R = INT_MAX ;
    Position U = pos, D = pos, L = pos, R = pos;
    U.setRow(pos.getRow()-1);
    D.setRow(pos.getRow()+1);
    L.setCol(pos.getCol()-1);
    R.setCol(pos.getCol()+1);

    if (map->isValid(U,this)) distance_U = ManhattanDistance(U,watson->getCurrentPosition());

    if (map->isValid(D,this)) distance_D = ManhattanDistance(D,watson->getCurrentPosition());

    if (map->isValid(L,this)) distance_L = ManhattanDistance(L,watson->getCurrentPosition());

    if (map->isValid(R,this)) distance_R = ManhattanDistance(R,watson->getCurrentPosition());

    
    int temp = min(min(distance_U,distance_D),min(distance_L,distance_R));
    if (temp == distance_U) {
        if (map->isValid(U,this)) {
            pos = U;
        }
        return pos;
    }
    else if (temp == distance_R) {
        if (map->isValid(R,this)) {
            pos = R;
        }
        return pos;
    }
    else if (temp == distance_D) {
        if (map->isValid(D,this)) {
            pos = D;
        }
        return pos;
    }
    else if (temp == distance_L) {
        if (map->isValid(L,this)) {
            pos = L;
        }
        return pos;
    }
    return pos;
}

RobotType RobotW::getType() const
{
    return W;
}

int RobotW::getDistance() const
{
    return ManhattanDistance(pos,watson->getCurrentPosition());
}

string RobotW::str() const
{
    return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(getDistance()) + "]";
}

void RobotW::move()
{
    Position next_position = getNextPosition();
    if (next_position.isEqual(Position::npos)) return;
    pos = next_position;
}

// RobotS

RobotS::RobotS(int index, const Position &init_pos, Map* map, Criminal* criminal, Sherlock* sherlock)
    : Robot(index, init_pos, map, S, criminal, "RobotS")
{
    this->sherlock = sherlock;
}

Position RobotS::getNextPosition()
{
    char dir;
    int distance_U = INT_MAX , distance_D = INT_MAX , distance_L = INT_MAX , distance_R = INT_MAX ;
    Position U = pos, D = pos, L = pos, R = pos;
    U.setRow(pos.getRow()-1);
    D.setRow(pos.getRow()+1);
    L.setCol(pos.getCol()-1);
    R.setCol(pos.getCol()+1);

    if (map->isValid(U,this)) distance_U = ManhattanDistance(U,sherlock->getCurrentPosition());

    if (map->isValid(D,this)) distance_D = ManhattanDistance(D,sherlock->getCurrentPosition());

    if (map->isValid(L,this)) distance_L = ManhattanDistance(L,sherlock->getCurrentPosition());

    if (map->isValid(R,this)) distance_R = ManhattanDistance(R,sherlock->getCurrentPosition());

    
    int temp = min(min(distance_U,distance_D),min(distance_L,distance_R));
    if (temp == distance_U) {
        if (map->isValid(U,this)) {
            pos = U;
        }
        return pos;
    }
    else if (temp == distance_R) {
        if (map->isValid(R,this)) {
            pos = R;
        }
        return pos;
    }
    else if (temp == distance_D) {
        if (map->isValid(D,this)) {
            pos = D;
        }
        return pos;
    }
    else if (temp == distance_L) {
        if (map->isValid(L,this)) {
            pos = L;
        }
        return pos;
    }
    return pos;
}

void RobotS::move()
{
    Position next_position = getNextPosition();
    if (next_position.isEqual(Position::npos)) return;
    pos = next_position;
}

string RobotS::str() const
{
    return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(getDistance()) + "]";    
}

RobotType RobotS::getType() const
{
    return S;
}

int RobotS::getDistance() const
{
    return ManhattanDistance(pos,sherlock->getCurrentPosition());
}

// RobotSW

RobotSW::RobotSW(int index, const Position &init_pos, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson)
    : Robot(index, init_pos, map, SW, criminal, "RobotSW")
{
    this->sherlock = sherlock;
    this->watson = watson;
}

Position RobotSW::getNextPosition()
{
    char dir;
    int distance_UU = INT_MAX , distance_DD = INT_MAX , distance_LL = INT_MAX , distance_RR = INT_MAX,
        distance_UR = INT_MAX , distance_UL = INT_MAX , distance_DL = INT_MAX , distance_DR = INT_MAX;
    Position UU = pos, DD = pos, LL = pos, RR = pos , UR = pos , UL = pos , DL = pos , DR = pos;
    UU.setRow(pos.getRow()-2);
    DD.setRow(pos.getRow()+2);
    LL.setCol(pos.getCol()-2);
    RR.setCol(pos.getCol()+2);

    UR.setRow(pos.getRow()-1);
    UR.setCol(pos.getCol()+1);

    UL.setRow(pos.getRow()-1);
    UL.setCol(pos.getCol()-1);

    DL.setRow(pos.getRow()+1);
    DL.setCol(pos.getCol()-1);

    DR.setRow(pos.getRow()+1);
    DR.setCol(pos.getCol()+1);

    if (map->isValid(UU,this)) distance_UU = ManhattanDistance(UU,watson->getCurrentPosition()) + ManhattanDistance(UU,sherlock->getCurrentPosition());

    if (map->isValid(DD,this)) distance_DD = ManhattanDistance(DD,watson->getCurrentPosition()) + ManhattanDistance(DD,sherlock->getCurrentPosition());

    if (map->isValid(LL,this)) distance_LL = ManhattanDistance(LL,watson->getCurrentPosition()) + ManhattanDistance(LL,sherlock->getCurrentPosition());

    if (map->isValid(RR,this)) distance_RR = ManhattanDistance(RR,watson->getCurrentPosition()) + ManhattanDistance(RR,sherlock->getCurrentPosition());

    if (map->isValid(UR,this)) distance_UR = ManhattanDistance(UR,watson->getCurrentPosition()) + ManhattanDistance(UR,sherlock->getCurrentPosition());

    if (map->isValid(UL,this)) distance_UL = ManhattanDistance(UL,watson->getCurrentPosition()) + ManhattanDistance(UL,sherlock->getCurrentPosition());

    if (map->isValid(DL,this)) distance_DL = ManhattanDistance(DL,watson->getCurrentPosition()) + ManhattanDistance(DL,sherlock->getCurrentPosition());

    if (map->isValid(DR,this)) distance_DR = ManhattanDistance(DR,watson->getCurrentPosition()) + ManhattanDistance(DR,sherlock->getCurrentPosition());

    int temp = min( min(min(distance_DD,distance_UU),min(distance_LL,distance_RR)),
                    min(min(distance_UR,distance_UL),min(distance_DL,distance_DR)) );
    if (temp == distance_UU) return UU;
    else if (temp == distance_UR) return UR;
    else if (temp == distance_RR) return RR;
    else if (temp == distance_DR) return DR;
    else if (temp == distance_DD) return DD;
    else if (temp == distance_DL) return DL;
    else if (temp == distance_LL) return LL;
    else if (temp == distance_UL) return UL;
    return Position::npos;
}
string RobotSW::str() const
{
    return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(getDistance()) + "]";
}
RobotType RobotSW::getType() const
{
    return SW;
}
int RobotSW::getDistance() const
{
    return ManhattanDistance(pos,sherlock->getCurrentPosition()) + ManhattanDistance(pos,watson->getCurrentPosition());
}
void RobotSW::move()
{
    Position next_position = getNextPosition();
    if (next_position.isEqual(Position::npos)) return;
    else pos = next_position;
}

// Base Item
BaseItem::BaseItem(ItemType in_type)
    : type(in_type)
{}

MagicBook::MagicBook()
    : BaseItem(MAGIC_BOOK)
{}

ItemType MagicBook::getType() const
{
    return MAGIC_BOOK;
}

string MagicBook::str() const
{
    return "MagicBook";
}

bool MagicBook::canUse(Character* obj, Robot* robot)
{
    if (obj->getEXP() <= 350 && robot == nullptr) return true;
    return false;
}

void MagicBook::use(Character* obj, Robot* robot)
{
    if (canUse(obj, robot)) {
        obj->setEXP(obj->getEXP()*1.25 - 0.00001);
    }
}
// CLASS: EnergyDrink

EnergyDrink::EnergyDrink()
    : BaseItem(ENERGY_DRINK)
{}

ItemType EnergyDrink::getType() const
{
    return ENERGY_DRINK;
}

string EnergyDrink::str() const
{
    return "EnergyDrink";
}

bool EnergyDrink::canUse(Character* obj, Robot* robot)
{
    if (obj->getHP() <= 100 && robot == nullptr) return true;
    return false;
}

void EnergyDrink::use(Character* obj, Robot* robot)
{
    if (canUse(obj, robot)) {
        obj->setHP(obj->getHP()*1.2 - 0.00001);
    }
}

// CLASS: FirstAid

FirstAid::FirstAid()
    : BaseItem(FIRST_AID)
{}

ItemType FirstAid::getType() const
{
    return FIRST_AID;
}

string FirstAid::str() const
{
    return "FirstAid";
}

bool FirstAid::canUse(Character* obj, Robot* robot)
{
    if ((obj->getHP() <= 100 || obj->getEXP() <= 350)&& robot!=nullptr) return true;
    return false;
}

void FirstAid::use(Character* obj, Robot* robot)
{
    if (canUse(obj, robot)) {
        obj->setHP(obj->getHP()*1.5 - 0.00001);
    }
}

// CLASS: ExcemptionCard

ExcemptionCard::ExcemptionCard()
    : BaseItem(EXCEMPTION_CARD)
{}

ItemType ExcemptionCard::getType() const
{
    return EXCEMPTION_CARD;
}

string ExcemptionCard::str() const
{
    return "ExcemptionCard";
}

bool ExcemptionCard::canUse(Character* obj, Robot* robot)
{
    if (obj->getObjectType() == SHERLOCK && robot != nullptr) {
        if (obj->getHP()%2) return true;
    }
    return false;
}

void ExcemptionCard::use(Character* obj, Robot* robot)
{

}

// *CLASS: PassingCard
/*PassingCard::PassingCard(int i, int j)
{
    int t = (i*11+j)%4;
    if (t == 0) challenge = "RobotS";
    else if (t == 1) challenge = "RobotC";
    else if (t == 2) challenge = "RobotSW";
    else challenge = "all";
    // TODO: constructor gán giá trị cho thuộc tính challenge (i,j là tọa độ)
}*/

PassingCard::PassingCard (string challenge)
    : BaseItem(PASSING_CARD)
{
    this->challenge = challenge;
}

ItemType PassingCard::getType() const
{
    return PASSING_CARD;
}

string PassingCard::str() const
{
    return "PassingCard";
}

bool PassingCard::canUse(Character* obj, Robot* robot)
{
    if (obj->getObjectType() == WATSON && robot != nullptr) {
        if (obj->getHP()%2 == 0) return true;
    }
    return false;
}

void PassingCard::use(Character* obj, Robot* robot)
{
    if (canUse(obj, robot)) {
        if (challenge != "all") {
            if (challenge == "RobotS") {
                if (robot->getType() != S) obj->setEXP(obj->getEXP()-50);
            }
            else if (challenge == "RobotC") {
                if (robot->getType() != C) obj->setEXP(obj->getEXP()-50);
            }
            else if (challenge == "RobotSW") {
                if (robot->getType() != SW) obj->setEXP(obj->getEXP()-50);
            }
        }
    }
    else obj->setEXP(obj->getEXP()-50);
    if (obj->getHP() < 0) obj->setHP(0);
}


Node::Node() : item(nullptr), next(nullptr) {}; 
Node::Node(BaseItem * item) : item(item), next(nullptr) {}; 
Node::~Node() {
    // delete this->item;
    // delete this->next;
}

/*Linkedlist::Linkedlist() : head(nullptr) {}
void Linkedlist::insert(BaseItem * item) {
    Node * newnode = new Node (item); 
    if (head == nullptr) {
        head = newnode; 
        return; 
    }
    Node * temp = head;
    head = newnode;
    newnode->next = temp;
    
    // delete newnode; 
    return; 
}*/

/*Linkedlist::~Linkedlist() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current; 
        current = current->next; 
        delete temp; 
    }
    head = 0; 
}*/

void BaseBag::deleteNode(Node * node) {
    Node * temp; 
    temp = node->next;
    node->next = temp->next;
    delete temp; 
}

void BaseBag::swapNode(Node * node) 
{
    count--;
    Node * current = head; 
    BaseItem * tempValue = current->item;
    current->item = node->item; 
    node->item = tempValue; 
    // delete current;
}

void BaseBag::deleteHead() 
{
    count--;
    Node * temp = head;
    head = head->next;
    delete temp;        
}

/*string Linkedlist::printList() {
    string s = ""; 
    Node * temp = head;
    if (head == nullptr) return s; 
    while (temp->next != nullptr) {
        s += temp->data->getType() + ","; 
        temp = temp->next; 
    }
    s += temp->data->getType(); 
    return s; 
}*/

BaseBag::BaseBag(Character * obj) : obj(obj) {}; 

BaseBag::~BaseBag()
{
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

Character * BaseBag::getObj() {
    return obj;
}

BaseItem* BaseBag::get() {
    Node* temp  = head;
    while (temp != nullptr) {
        if (temp->item->canUse(this->getObj(), nullptr)) {
            BaseItem * back = temp->item;
            this->swapNode(temp); 
            this->deleteHead();
            return back; 
        } 
        temp = temp->next; 
    }
    // delete temp; 
    return nullptr; 
}

BaseItem* BaseBag::get(ItemType itemtype) {
    Node* head = head; 
    while (head != nullptr) {
        if (itemtype == EXCEMPTION_CARD || itemtype == PASSING_CARD) {
            if (head->item->getType() == itemtype) {
                BaseItem * back = head->item;
                this->swapNode(head); 
                this->deleteHead();
                return back; 
            }
        }
        else {
            if (head->item->canUse(this->getObj(), nullptr) && head->item->getType() == itemtype) {
                BaseItem * back = head->item;
                this->swapNode(head); 
                this->deleteHead();
                return back; 
            }
        }
        head = head->next;
    }
    // delete head; 
    return nullptr; 
}

string BaseBag::str() const
{
    string ans = "BaseBag[count=" + to_string(count);
    Node* current = head;
    while (current != nullptr)
    {
        ans += ";" + current->item->str();
        current = current->next;
    }
    ans += "]";
    return ans;
}


bool BaseBag::insert(BaseItem* item)
{
    if (count >= capacity) return false; 
    Node * newnode = new Node (item); 
    if (head == nullptr) {
        head = newnode; 
        count++;
        return true; 
    }
    Node * temp = head;
    head = newnode;
    newnode->next = temp;
    count++;
    return true;
    // delete newnode; 
}

SherlockBag::SherlockBag(Sherlock * sherlock)
    : BaseBag(sherlock)
{
    count = 0;
    capacity = 13;
}

WatsonBag::WatsonBag(Watson * watson)
    : BaseBag(watson)
{
    count = 0;
    capacity = 15;
}


// Study In Pink Program

/*StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    this->config = new Configuration(config_file_path);
    this->map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    this->arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    this->sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    this->watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    this->criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);
    this->sherlockbag = new SherlockBag(sherlock);
    this->watsonbag = new WatsonBag(watson);

    this->arr_mv_objs->add(criminal);
    this->arr_mv_objs->add(sherlock);
    this->arr_mv_objs->add(watson);
}

StudyPinkProgram::~StudyPinkProgram()
{
    delete this->config;
    delete this->map;
    delete this->arr_mv_objs;
    delete this->sherlock;
    delete this->watson;
    delete this->criminal;
    delete this->sherlockbag;
    delete this->watsonbag;
}*/

StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    this->config = new Configuration(config_file_path);
    this->map = new Map(config->getNumRows(), config->getNumCols(), config->getNumWalls(), config->getArrWalls(), config->getNumFakeWalls(), config->getArrFakeWalls());
    this->sherlock = new Sherlock(1, config->getSherlockMovingRule(), config->getSherlockInitPos(), map, config->getSherlockInitHp(), config->getSherlockInitExp());
    this->watson = new Watson(2, config->getWatsonMovingRule(), config->getWatsonInitPos(), map, config->getWatsonInitHp(), config->getWatsonInitExp());
    this->criminal = new Criminal(0, config->getCriminalInitPos(), map, sherlock, watson);
    this->arr_mv_objs = new ArrayMovingObject(config->getMaxNumMovingObjects());
    this->sherlockbag = new SherlockBag(sherlock);
    this->watsonbag = new WatsonBag(watson);

    this->arr_mv_objs->add(sherlock);
    this->arr_mv_objs->add(watson);
    this->arr_mv_objs->add(criminal);
}

StudyPinkProgram::~StudyPinkProgram()
{
    delete this->config;
    delete this->map;
    delete this->sherlock;
    delete this->watson;
    delete this->criminal;
    delete this->arr_mv_objs;
    delete this->sherlockbag;
    delete this->watsonbag;
}

void StudyPinkProgram::run(bool verbose) {}

/*void StudyPinkProgram::run(bool verbose)
{
    for (int istep = 0; istep < config->getNumSteps(); ++istep)
    {
        for (int i = 0; i < arr_mv_objs->MovingObjectCount(); ++i)
        {
            arr_mv_objs->get(i)->move();

            if (isStop())
            {
                printStep(istep);
                break;
            }

            if (verbose)
            {
                printStep(istep);
            }

            MovingObjectType mv_obj_type = arr_mv_objs->get(i)->getObjectType();

            if (mv_obj_type == SHERLOCK) {
                Sherlock *sherlock = dynamic_cast<Sherlock *>(arr_mv_objs->get(i));
                sherlock->action(arr_mv_objs, sherlockbag, watsonbag);
            }

            if (mv_obj_type == WATSON) {
                Watson *watson = dynamic_cast<Watson *>(arr_mv_objs->get(i));
                watson->action(arr_mv_objs, sherlockbag, watsonbag);
            }

            if (mv_obj_type == CRIMINAL) {
                criminal->create(arr_mv_objs, sherlock, watson);
            }
            Sherlock *sherlock = dynamic_cast<Sherlock *>(arr_mv_objs->get(i));
            if (sherlock != nullptr)
            {
                sherlock->action(arr_mv_objs, sherlockbag, watsonbag);
            }
            Watson *watson = dynamic_cast<Watson *>(arr_mv_objs->get(i));
            if (watson != nullptr)
            {
                watson->action(arr_mv_objs, sherlockbag, watsonbag);
            }
            Criminal *criminal = dynamic_cast<Criminal *>(arr_mv_objs->get(i));
            if (criminal != nullptr)
            {
                criminal->create(arr_mv_objs, sherlock, watson);
            }
        }
    }
    printResult();
}*/

bool StudyPinkProgram::isStop() const
{
    if(sherlock->getHP() == 0 || watson->getHP() == 0 
        || sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()) 
        || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) return true;
    return false;
}


void StudyPinkProgram::printResult() const {
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        cout << "Sherlock caught the criminal" << endl;
    }
    else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) {
        cout << "Watson caught the criminal" << endl;
    }
    else {
        cout << "The criminal escaped" << endl;
    }
}

void StudyPinkProgram::printStep(int si) const {
    cout << "Step: " << setw(4) << setfill('0') << si
        << "--"
        << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////








