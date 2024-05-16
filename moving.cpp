#include "moving.h"

int ManhattanDistance(const Position &pos1, const Position &pos2)
{
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
};









// Definition of classes

// Map part

const Position Position::npos(-1,-1);

Position::Position(int r, int c)
{
    // TODO: constructor
    this->r = r;
    this->c = c;
}

/*Position::Position(const string &str_pos)
{
    int pos = str_pos.find(',');
    r = stoi(str_pos.substr(1,pos-1));
    c = stoi(str_pos.substr(pos+1,str_pos.size()-3));    
}*/

Position::Position(const string &str_pos)
{
    stringstream ss(str_pos); // Chuyển chuỗi thành luồng.

    string item;

    getline(ss, item, '('); // Loại bỏ ký tự '('

    getline(ss, item, ','); // Lấy giá trị r (hàng)
    this->r = stoi(item);   // Chuyển chuỗi thành số nguyên và gán cho r

    getline(ss, item, ')'); // Lấy giá trị c (cột)
    this->c = stoi(item);  // Chuyển chuỗi thành số nguyên và gán cho c
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
{
    this->num_rows = num_rows;
    this->num_cols = num_cols;    
    map = new MapElement**[num_rows];
    for(int i = 0; i < num_rows; i++) {
        map[i] = new MapElement*[num_cols];
        for (int j = 0; j < num_cols; ++j)
        {
            map[i][j] = new Path();
        }
    }

    for(int i = 0; i < num_fake_walls; i++) {
        int r = array_fake_walls[i].getRow();
        int c = array_fake_walls[i].getCol();
        delete map[r][c];
        map[r][c] = new FakeWall((r * 257 + c * 139 + 89) % 900 + 1);
    }

    for(int i = 0; i < num_walls; i++) {
        int r = array_walls[i].getRow();
        int c = array_walls[i].getCol();
        delete map[r][c];
        map[r][c] = new Wall();
    }
}

Map::~Map()
{
    for(int i = 0; i < num_rows; i++) {
        for(int j = 0; j < num_cols; j++) {
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



Position MovingObject::getCurrentPosition() const
{
    return pos;
}

// Class Character

Character::Character(int index, const Position pos, Map *map, const string &name)
    : MovingObject(index, pos, map, name) {}

// Class Sherlock

Sherlock::Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Sherlock"), moving_rule(moving_rule)
{
    // this->Bag = new SherlockBag(this);
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
    char dir = moving_rule[index_moving_rule++ % moving_rule.length()];
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

/*bool Sherlock::meet(Robot* robot)
{
    if (robot->getDistance() == 0) {
        RobotType type = robot->getType();
        BaseItem* item = Bag->get(EXCEMPTION_CARD);
        if (type == S) {
            if (exp > 400) {
                Bag->insert(robot->getItem());
                delete robot;
            }
            else {
                if (item != nullptr) {
                    ExcemptionCard* excemption_card = dynamic_cast <ExcemptionCard* > (item);
                    if (!excemption_card->canUse(this,robot)) exp = exp*0.9;
                }
            }
            item = Bag->get();
            item->use(this,nullptr);
            return false;
        }
        else if (type == W) {
            Bag->insert(robot->getItem());
            delete robot;
            item = Bag->get();
            item->use(this,nullptr);
            return false;
        }
        else if (type == SW) {
            if (exp > 300 && hp > 335) {
                Bag->insert(robot->getItem());
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
            item = Bag->get();
            item->use(this,nullptr);
            return false;
        }
        else if (type == C) {
            if (exp > 500) {
                // sherlock pos = criminal pos
                this->pos = robot->getNextPosition();
                return true;
            }
            else {
                Bag->insert(robot->getItem());
                delete robot;
            }
            item = Bag->get();
            item->use(this,nullptr);
            return false;
        }
        
    }
    return false;
}

bool Sherlock::meet(Watson* watson)
{
    if (pos.isEqual(watson->getCurrentPosition())) {
        // if (sherlock)
    }
    return false;
}*/

// Watson

Watson::Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Watson"), moving_rule(moving_rule)
{
    // this->Bag = new WatsonBag(this);
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
    char dir = moving_rule[index_moving_rule++ % moving_rule.size()];
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

/*void Watson::meet(Robot* robot)
{
    if (robot->getDistance() == 0) {
        BaseItem* item = Bag->get();
        RobotType type = robot->getType();
        if (type == S) {
            Bag->insert(robot->getItem());
            delete robot;
        }
        else if (type == W) {
            if (hp > 350) {
                Bag->insert(robot->getItem());
                delete robot;
            }
            else hp *= 0.95;
        }
        else if (type == SW) {
            if (item->getType() == PASSING_CARD) {
                if (item->canUse(this,robot)) {
                    item->use(this,robot);
                    delete robot;
                }
            }
            else {
                if (exp > 600 && hp > 165) {
                    Bag->insert(robot->getItem());
                    delete robot;
                }
                else {
                    exp *= 0.85;
                    hp *= 0.85;
                }
            }
            
        }
        else if (type == C) {
            Bag->insert(robot->getItem());
            delete robot;
        }
        
    }

}

void Watson::meet(Sherlock* sherlock)
{

}*/

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



MovingObjectType Criminal::getObjectType() const
{
    return CRIMINAL;
}

// Array Moving Object

ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    arr_mv_objs = new MovingObject*[capacity];
    for (int i = 0; i < capacity; i++) {
        arr_mv_objs[i] = nullptr;
    }

}

ArrayMovingObject::~ArrayMovingObject()
{
    delete[] arr_mv_objs;
}
 
bool ArrayMovingObject::isFull() const
{
    return count == capacity;
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


int ArrayMovingObject::size() const
{
    return capacity;
}




Robot::Robot(int index, const Position &pos, Map* map, RobotType robot_type, Criminal* criminal, const string &name)
    : MovingObject(index, pos, map, "Robot")
{
    this->robot_type = robot_type;
    this->criminal = criminal;

    int p = pos.getRow() * pos.getCol();
    if (p == 0) p = 0;
    else if (p%9 == 0) p = 9;
    else p = p%9;

    /*switch(p) {
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
    }*/
}

/*Robot::~Robot()
{
    delete item;
}*/

Robot* Robot::create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson)
{
    if (criminal->getCount() == 2) return new RobotC(index, criminal->getCurrentPosition(), map, C,criminal);

    if (criminal->isCreatedRobotNext()) {
        int criminal_sherlock_distance = ManhattanDistance(criminal->getCurrentPosition(), sherlock->getCurrentPosition());
        int criminal_watson_distance =  ManhattanDistance(criminal->getCurrentPosition(), watson->getCurrentPosition());
        int temp = min(criminal_sherlock_distance,criminal_watson_distance);

        if (temp == criminal_sherlock_distance && temp == criminal_watson_distance) {
            return new RobotSW(index, criminal->getCurrentPosition(), map, SW, criminal, sherlock, watson);
        }
        else if (temp == criminal_sherlock_distance) {
            return new RobotS(index, criminal->getCurrentPosition(), map, S, criminal, sherlock);
        }
        else {
            return new RobotW(index, criminal->getCurrentPosition(), map, W, criminal, watson);
        }
    }
    return NULL;
}

MovingObjectType Robot::getObjectType() const
{
    return ROBOT;
}

/*BaseItem* Robot::getItem() const
{
    return item;
}*/

// RobotC

RobotC::RobotC(int index, const Position &init_pos, Map* map, RobotType robot_type, Criminal* criminal)
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

RobotW::RobotW(int index, const Position &init_pos, Map* map, RobotType robot_type, Criminal* criminal, Watson* watson)
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

RobotS::RobotS(int index, const Position &init_pos, Map* map, RobotType robot_type, Criminal* criminal, Sherlock* sherlock)
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

RobotSW::RobotSW(int index, const Position &init_pos, Map* map, RobotType robot_type, Criminal* criminal, Sherlock* sherlock, Watson* watson)
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





// Config part

/*Configuration::Configuration(const string & filepath)
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
                for (int i = 0; i < num_walls; i++) {
                    getline(ss,item, ';');
                    arr_walls[i] = Position(item);
                }
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
                for (int i = 0; i < num_fake_walls; i++) {
                    getline(ss,item, ';');
                    arr_fake_walls[i] = Position(item);
                }
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
}*/

Configuration::Configuration(const string &filepath)
{
    //    TODO: constructor
    ifstream readInputFile (filepath); 
string input_line[15];
for (int b=0;b<15;++b){
getline(readInputFile,input_line[b]);
}

//MAP_NUM_ROWS
stringstream ss1(input_line[0]);
string chu1; 
getline(ss1,chu1,'=');
ss1>>map_num_rows;


 
//MAP_NUM_COLS
stringstream ss2(input_line[1]);
string chu2; 
getline(ss2,chu2,'=');
ss2>>map_num_cols;


//MAX_NUM_MOVING_OBJECTS
stringstream ss3(input_line[2]);
string chu3; 
getline(ss3,chu3,'=');
ss3>>max_num_moving_objects;


// ARRAY_WALLS
string dongarraywall = input_line[3];
num_walls=0;
    for (int i = 0; i < dongarraywall.length(); ++i) {
        if (dongarraywall[i] == ',') {
            (num_walls)++;
        }
    }
this->num_walls=num_walls;
arr_walls = new Position[num_walls];
        stringstream ssarw(dongarraywall);
        char ch1;
        int a = 0;
        while (ssarw >> ch1) {
            if (ch1 == '(') {
                int num1, num2;
                ssarw >> num1;
                ssarw.ignore();
                ssarw >> num2;
                arr_walls[a] = Position(num1, num2);
                a++;
            }
        }

// ARRAY_FAKE_WALLS
string dongarrayfakewall = input_line[4];
num_fake_walls=0;
for (int i = 0; i < dongarrayfakewall.length(); ++i) {
    if (dongarrayfakewall[i] == ',') {
        num_fake_walls++;
    }
}

arr_fake_walls = new Position[num_fake_walls];


stringstream ssarfw(dongarrayfakewall);
char ch2;
int b = 0;

while (ssarfw >> ch2) {
    if (ch2 == '(') {
        int num3, num4;

        // Extract the pair of numbers
        ssarfw >> num3;
        ssarfw.ignore(); // Ignore the comma
        ssarfw >> num4;

        // Store the numbers in the array
        arr_fake_walls[b] = Position(num3, num4);

        // Move to the next pair
        b++;
    }
}


// SHERLOCK_MOVING_RULE
int daubang6 = input_line[5].find("=");
sherlock_moving_rule = input_line[5].substr(daubang6 +1);

//SHERLOCK_INIT_POS
int sherlock_daungoac1= input_line[6].find("(");
string sherlock_init_position = input_line[6].substr(sherlock_daungoac1+1);
    stringstream ss_sherlock_init_position(sherlock_init_position);
    int sherlock_x,sherlock_y;
    ss_sherlock_init_position >> sherlock_x;
    ss_sherlock_init_position.ignore();
    ss_sherlock_init_position >> sherlock_y;
    ss_sherlock_init_position.ignore();
sherlock_init_pos =Position (sherlock_x,sherlock_y);

// SHERLOCK_INIT_HP
int daubang8 = input_line[7].find("=");
string sodong8 = input_line[7].substr(daubang8 +1);
stringstream ss_sherlock_init_hp(sodong8);
ss_sherlock_init_hp>>sherlock_init_hp;


// SHERLOCK_INIT_EXP
int daubang9 = input_line[8].find("=");
string sodong9 = input_line[8].substr(daubang9 +1);
stringstream ss_sherlock_init_exp(sodong9);
ss_sherlock_init_exp>>sherlock_init_exp;

// WATSON_MOVING_RULE
int daubang10 = input_line[9].find("=");
watson_moving_rule = input_line[9].substr(daubang10 +1);
int watson_daungoac1= input_line[10].find("(");

string watson_init_position = input_line[10].substr(watson_daungoac1+1);
    stringstream ss_watson_init_position(watson_init_position);
    int watson_x, watson_y;
    ss_watson_init_position >> watson_x;
    ss_watson_init_position.ignore();
    ss_watson_init_position >> watson_y;
    ss_watson_init_position.ignore();

 watson_init_pos = Position(watson_x,watson_y);
//WATSON_INIT_HP
int daubang12 = input_line[11].find("=");
string sodong12 = input_line[11].substr(daubang12 +1);
stringstream ss_watson_init_hp(sodong12);
ss_watson_init_hp>>watson_init_hp;

//WATSON_INIT_EXP
int daubang13 = input_line[12].find("=");
string sodong13 = input_line[12].substr(daubang13 +1);
stringstream ss_watson_init_exp(sodong13);
ss_watson_init_exp>>watson_init_exp;

//CRIMINAL_INIT_POS
int criminal_daungoac1= input_line[13].find("(");
string criminal_init_position = input_line[13].substr(criminal_daungoac1+1);
    stringstream ss_criminal_init_position(criminal_init_position);
    int criminal_x,criminal_y;
    ss_criminal_init_position >> criminal_x;
    ss_criminal_init_position.ignore();
    ss_criminal_init_position >> criminal_y;
    ss_criminal_init_position.ignore();
 criminal_init_pos =Position(criminal_x,criminal_y);
 

//NUM_STEPS
int daubang15 = input_line[14].find("=");
string sodong15 = input_line[14].substr(daubang15 +1);
stringstream ss_num_steps(sodong15);
ss_num_steps>>num_steps;




readInputFile.close();
}

Configuration::~Configuration()
{
    // TODO: destructor
        delete[] arr_walls;
        delete[] arr_fake_walls;
}

string Configuration::str() const
{
    // TODO: trả về chuỗi mô tả file
    stringstream ss_Config;
    ss_Config<<"Configuration["<<endl;
    ss_Config << "MAP_NUM_ROWS=" << map_num_rows << endl;
    ss_Config << "MAP_NUM_COLS=" << map_num_cols << endl;
    ss_Config << "MAX_NUM_MOVING_OBJECTS=" << max_num_moving_objects << endl;
    ss_Config << "NUM_WALLS=" << num_walls << endl;
    ss_Config << "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; ++i) {
        ss_Config << "(" << arr_walls[i].getRow() << "," << arr_walls[i].getCol() << ")";
        if (i < num_walls - 1) {
            ss_Config << ";";
        }
    }
    ss_Config << "]" << endl;

    ss_Config << "NUM_FAKE_WALLS=" << num_fake_walls << endl;
    ss_Config << "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; ++i) {
        ss_Config << "(" << arr_fake_walls[i].getRow() << "," << arr_fake_walls[i].getCol() << ")";
        if (i < num_fake_walls - 1) {
            ss_Config << ";";
        }
    }
    ss_Config << "]" << endl;

    ss_Config << "SHERLOCK_MOVING_RULE=" << sherlock_moving_rule << endl;
    ss_Config << "SHERLOCK_INIT_POS=(" << sherlock_init_pos.getRow() << "," << sherlock_init_pos.getCol() << ")" << endl;
    ss_Config << "SHERLOCK_INIT_HP=" << sherlock_init_hp << endl;
    ss_Config << "SHERLOCK_INIT_EXP=" << sherlock_init_exp << endl;
ss_Config << "WATSON_MOVING_RULE=" << watson_moving_rule << endl;
    ss_Config << "WATSON_INIT_POS=(" << watson_init_pos.getRow() << "," << watson_init_pos.getCol() << ")" << endl;
    ss_Config << "WATSON_INIT_HP=" << watson_init_hp << endl;
    ss_Config << "WATSON_INIT_EXP=" << watson_init_exp << endl;
    ss_Config << "CRIMINAL_INIT_POS=(" << criminal_init_pos.getRow() << "," << criminal_init_pos.getCol() << ")" << endl;
    ss_Config << "NUM_STEPS=" << num_steps << endl;
    ss_Config<<"]";

    return ss_Config.str();
}























// WATSON_INIT_POS

/*Configuration::~Configuration()
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
}*/





StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    config = new Configuration(config_file_path);
    map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
    arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
    sherlock = new Sherlock(1, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
    watson = new Watson(2, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
    criminal = new Criminal(0, config->criminal_init_pos, map, sherlock, watson);

    arr_mv_objs->add(criminal);
    arr_mv_objs->add(sherlock);
    arr_mv_objs->add(watson);
}

StudyPinkProgram::~StudyPinkProgram()
{
    // TODO: hủy những đối tượng cấp phát
    // * Nếu thực hiện hủy toàn bộ trong ArrayMovingObject thì không cần delete criminal, sherlock và watson
    delete map;
    delete config;
    delete arr_mv_objs;
    //delete criminal;
    //delete sherlock;
    //delete watson;
}

void StudyPinkProgram::printMap(ofstream &OUTPUT, int roundSize) const
{
    for (int i = -1; i < config->map_num_cols; i++)
    {
        if (i == -1)
            OUTPUT << setw(5) << ""
                   << "|";
        else
            OUTPUT << setw(5) << i << "|";
    }
    OUTPUT << endl;
    for (int i = 0; i < config->map_num_rows; i++)
    {
        OUTPUT << setw(5) << i << "|";
        for (int j = 0; j < config->map_num_cols; j++)
        {
            int idx = map->getElementType(i, j);
            string nameElement[3] = {"     ", "IIIII", "-----"};
            string nameChar[4] = {"S", "W", "C", "R"};
            string robotName[4] = {"c", "s", "w", "2"};
            string cellValue = nameElement[idx];
            Position charPos(i, j);
            for (int k = 0; k < roundSize; k++)
            {
                if (arr_mv_objs->get(k)->getCurrentPosition().isEqual(charPos))
                {
                    if (cellValue == "     " || cellValue == "-----" || cellValue == "IIIII")
                        cellValue = "";
                    idx = arr_mv_objs->get(k)->getObjectType();
                    if (idx == 3)
                    {
                        MovingObject *temp = arr_mv_objs->get(k);
                        cellValue += robotName[dynamic_cast<Robot *>(temp)->getType()];
                        continue;
                    }
                    cellValue += nameChar[idx];
                }
            }
            if (!(cellValue == "     " || cellValue == "-----" || cellValue == "IIIII"))
                cellValue = "(" + cellValue + ")";
            OUTPUT << setw(5) << cellValue << "|";
        }
        OUTPUT << endl;
    }
}

void StudyPinkProgram::run(ofstream &OUTPUT)
{
    if (!OUTPUT.is_open())
    {
        cout << "Can't open file" << endl;
        return;
    }
    OUTPUT << config->str() << endl;
    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        OUTPUT << "__________________NEW STEP__________________" << endl;
        MovingObject *robot = nullptr;
        int roundSize = arr_mv_objs->size();
        for (int i = 0; i < roundSize; ++i)
        {

            OUTPUT << "ROUND : " << istep << " - TURN : " << i << endl;
            stringstream ss(arr_mv_objs->str());
            string lineArr = "";
            getline(ss, lineArr, 'C');
            OUTPUT << lineArr << "]" << endl;
            getline(ss, lineArr, ']');
            OUTPUT << "\tC" << lineArr.substr(1) << "]" << endl;
            while (getline(ss, lineArr, ']'))
            {
                if (lineArr.length() > 0)
                    OUTPUT << "\t" << lineArr.substr(1) << "]" << endl;
            }
            if (i == 0)
                OUTPUT << "Criminal current count : " << criminal->getCount() << endl;
            if (i == 1)
                OUTPUT << "Sherlock move direction : " << config->sherlock_moving_rule[istep % config->sherlock_moving_rule.length()] << endl;
            if (i == 2)
                OUTPUT << "Watson move direction : " << config->watson_moving_rule[istep % config->watson_moving_rule.length()] << endl;

            if (arr_mv_objs->get(i)->getObjectType() == MovingObjectType::CRIMINAL)
            {
                robot = Robot::create(arr_mv_objs->size(), map, criminal, sherlock, watson);
            }
            arr_mv_objs->get(i)->move();
            if (i == 0)
                OUTPUT << "Criminal count after moving : " << criminal->getCount() << endl;

            printMap(OUTPUT, roundSize);
            OUTPUT << "*********************************************************" << endl;
        }
        if (robot != nullptr)
        {
            if (criminal->getCount() % 3 == 0 && criminal->getCount() > 0)
            {
                string robotType[4] = {"C", "S", "W", "SW"};
                OUTPUT << "Robot " << robotType[dynamic_cast<Robot *>(robot)->getType()] << " has been created" << endl;
                arr_mv_objs->add(robot);
            }
            else
            {
                // delete robot;
            }
        }
    }
}