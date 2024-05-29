#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Supporting Functions

int ManhattanDistance(const Position &pos1, const Position &pos2)
{
    return abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
};

// Declaration of Classes
const Position Position::npos = Position(-1, -1);

//  Class MapElement
MapElement::MapElement(ElementType in_type)
    : type(in_type) {}

MapElement::~MapElement() {}

ElementType MapElement::getType() const
{
    return type;
}

//  Class Path

Path::Path()
    : MapElement(PATH) {}

// Class Wall

Wall::Wall()
    : MapElement(WALL) {}

//  Class FakeWall 

FakeWall::FakeWall(int in_req_exp)
    : MapElement(ElementType::FAKE_WALL)
{
    req_exp = in_req_exp;
}

int FakeWall::getReqExp() const
{
    return req_exp;
}

//  Class Position

Position::Position(int r, int c)
    : r(r), c(c)
{}

Position::Position(const string & str_pos)
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
bool Position::isEqual(int in_r, int in_c) const
{
    return (in_r == r) && (in_c == c);
}

bool Position::isEqual(Position pos) const
{
    return (pos.getRow() == r) && (pos.getCol() == c);
}

//  Class MovingObject

MovingObject::MovingObject(int index, const Position pos, Map* map, const string &name)
    : index(index), pos(pos), map(map), name(name) {}

MovingObject::~MovingObject(){};

Position MovingObject::getCurrentPosition() const
{
    return pos;
}

//  Class Character

Character::Character(int index, const Position pos, Map* map, const string &name)
    : MovingObject(index, pos, map, name) {}

// Class Sherlock

Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map* map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Sherlock")
{
    this->moving_rule = moving_rule;
    index_moving_rule = 0;
    bag = new SherlockBag(this);
    if (init_hp > 500) hp = 500;
    else if (init_hp < 0) hp = 0;
    else hp = init_hp;

    if (init_exp > 900) exp = 900;
    else if (init_exp < 0) exp = 0;
    else exp = init_exp;
}

Sherlock::~Sherlock() 
{
    delete bag;
}

Position Sherlock::getNextPosition()
{
    char dir = moving_rule[index_moving_rule % moving_rule.length()];
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
    Position next_position = this->getNextPosition();
    index_moving_rule++;
    if(next_position.isEqual(Position::npos) || exp == 0) return;
    else this->pos = next_position;
}

string Sherlock::str() const
{
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

MovingObjectType Sherlock::getObjectType() const
{
    return SHERLOCK;
}

void Sherlock::sethp(int init_hp) 
{
    if (init_hp > 500) hp = 500;
    else if (init_hp < 0) hp = 0;
    else hp = init_hp;
}

void Sherlock::setexp(int init_exp) 
{
    if (init_exp < 0) exp = 0;
    else if (init_exp > 900) exp = 900;
    else exp = init_exp;
}

int Sherlock::getexp() const
{
    return exp;
}

int Sherlock::gethp() const
{
    return hp;
}

BaseBag* Sherlock::getBag() const
{
    return bag;
}

bool Sherlock::meet(RobotS* robotS)
{
    if (hp%2) {
        BaseItem* temp = bag->get(EXCEMPTION_CARD);
        if (temp != nullptr) {
            // temp->use(this,robotS);
            BaseItem* item = robotS->getItem();
            if (exp > 400) { 
                bag->insert(item);
            }
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth ->use(this,nullptr);
            return false;
        }
        else {
            BaseItem* item = robotS->getItem();
            if (exp > 400) { 
                bag->insert(item);
            }
            else {
                exp = ceil(exp*0.9 - 0.0001);
            }
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth ->use(this,nullptr);
            return false;
        }
    }

    BaseItem* item = robotS->getItem();
    if (exp > 400) { 
        bag->insert(item);
    }
    else {
        exp = ceil(exp*0.9 - 0.0001);
    }
    BaseItem* sth = bag->get();
    if (sth != nullptr) sth ->use(this,nullptr);
    return false;
}

bool Sherlock::meet(RobotW* robotW)
{
    if (hp%2) {
        BaseItem* temp = bag->get(EXCEMPTION_CARD);
        if (temp != nullptr) {
            // temp->use(this,robotS);
            BaseItem* item = robotW->getItem();
            bag->insert(item);
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth->use(this,nullptr);
            return false;
        }
        else {
            BaseItem* item = robotW->getItem();
            bag->insert(item);
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth ->use(this,nullptr);
            return false;
        }
    }
    BaseItem* item = robotW->getItem();
    bag->insert(item);
    BaseItem* sth = bag->get();
    if (sth != nullptr) sth ->use(this,nullptr);
    return false;
}

bool Sherlock::meet(RobotSW* robotSW)
{
    if (hp%2) {
        BaseItem* temp = bag->get(EXCEMPTION_CARD);
        if (temp != nullptr) {
            BaseItem* item = robotSW->getItem();
            if (exp > 300 && hp > 335) {
                bag->insert(item);
            }
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth->use(this,nullptr);
            return false;
        }
        else {
            BaseItem* item = robotSW->getItem();
            if (exp > 300 && hp > 335) {
                bag->insert(item);
            }
            else {
                exp = ceil(exp*0.85 - 0.0001);
                hp = ceil(hp*0.85 - 0.0001);
            }
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth->use(this,nullptr);
            return false;
        }
    }
    
    BaseItem* item = robotSW->getItem();
    if (exp > 300 && hp > 335) {
        bag->insert(item);
    }
    else {
        exp = ceil(exp*0.85 - 0.0001);
        hp = ceil(hp*0.85 - 0.0001);
    }
    BaseItem* sth = bag->get();
    if (sth != nullptr) sth->use(this,nullptr);
    return false;
}

bool Sherlock::meet(RobotC* robotC)
{
    if (hp%2) {
        BaseItem* temp = bag->get(EXCEMPTION_CARD);
        if (temp != nullptr) {
            if (exp > 500) {
                pos = robotC->CriminalPos();
                return true;
            }
            BaseItem* item = robotC->getItem();
            bag->insert(item);
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth->use(this,nullptr);
            return false;
        }
        else {
            BaseItem* item = robotC->getItem();
            if (exp > 500) {
                pos = robotC->CriminalPos();
                return true;
            }
            else bag->insert(item);

            BaseItem* sth = bag->get();
            if (sth != nullptr) sth->use(this,nullptr);
            return false;
        }
    }
    
    BaseItem* item = robotC->getItem();
    if (exp > 500) {
        pos = robotC->CriminalPos();
        return true;
    }
    else bag->insert(item);

    BaseItem* sth = bag->get();
    if (sth != nullptr) sth->use(this,nullptr);
    return false;
}

bool Sherlock::meet(Watson* watson)
{
    if (!(bag->checkItem(PASSING_CARD) && watson->getBag()->checkItem(EXCEMPTION_CARD))) return false;

    BaseItem* temp_sherItem = bag->get(PASSING_CARD);
    BaseItem* temp_watsonItem = watson->getBag()->get(EXCEMPTION_CARD);
    if (temp_sherItem != NULL && temp_watsonItem != NULL)
    {
        while (temp_sherItem != NULL && temp_watsonItem != NULL)
        {
            watson->getBag()->insert(temp_sherItem);
            bag->insert(temp_watsonItem);
            temp_sherItem = bag->get(PASSING_CARD);
            temp_watsonItem = watson->getBag()->get(EXCEMPTION_CARD);
        }
    }
    return false;
}

// Class Watson

Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map* map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Watson")
{
    this->moving_rule = moving_rule;
    index_moving_rule = 0;
    bag = new WatsonBag(this);
    if (init_hp > 500) hp = 500;
    else if (init_hp < 0) hp = 0;
    else hp = init_hp;

    if (init_exp > 900) exp = 900;
    else if (init_exp < 0) exp = 0;
    else exp = init_exp;
}

Watson::~Watson()
{
    delete bag;
}

MovingObjectType Watson::getObjectType() const
{
    return WATSON;
}

Position Watson::getNextPosition() {
    char dir = moving_rule[index_moving_rule % moving_rule.size()];
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
    index_moving_rule++;
    if(next_position.isEqual(Position::npos) || exp == 0) return;
    pos = next_position;
}

string Watson::str() const
{
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";  
}

int Watson::getexp() const
{
    return exp;
}

int Watson::gethp() const
{
    return hp;
}

void Watson::setexp(int init_exp) 
{
    if (init_exp < 0) exp = 0;
    else if (init_exp > 900) exp = 900;
    else exp = init_exp;
}

void Watson::sethp(int init_hp) 
{
    if (init_hp > 500) hp = 500;
    else if (init_hp < 0) hp = 0;
    else hp = init_hp;
}

BaseBag* Watson::getBag() const
{
    return bag;
}

bool Watson::meet(RobotS* robotS)
{
    // TODO: Xử lý trao đổi khi gặp robot S
    if (hp%2 == 0) {
        BaseItem* temp =  bag->get(PASSING_CARD);
        if (temp != nullptr) temp->use(this, robotS);
    }
    BaseItem* sth = bag->get();
    if (sth != nullptr) sth->use(this,nullptr);
    return false;
}
bool Watson::meet(RobotW* robotW)
{
    // TODO: Xử lý trao đổi khi gặp robot W
    if (hp%2 == 0) {
        BaseItem* temp =  bag->get(PASSING_CARD);
        if (temp != nullptr) {
            temp->use(this,robotW);
            BaseItem* item = robotW->getItem();
            bag->insert(item);
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth->use(this,nullptr);
            return false;
        }
    }
    BaseItem* item = robotW->getItem();
    if (hp > 350) {
        bag->insert(item);
    }
    else {
        hp = ceil(hp*0.95 - 0.0001);
    }
    BaseItem* sth = bag->get();
    if (sth != nullptr) sth->use(this,nullptr);
    return false;
}
bool Watson::meet(RobotSW* robotSW)
{
    // TODO: Xử lý trao đổi khi gặp robot SW
    if (hp%2 == 0) {
        BaseItem* temp =  bag->get(PASSING_CARD);
        if (temp != nullptr) {
            temp->use(this,robotSW);
            BaseItem* item = robotSW->getItem();
            bag->insert(item);
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth->use(this,nullptr);
            return false;
        }
    }
    BaseItem* item = robotSW->getItem();
    if (exp > 600 && hp > 165) {
        bag->insert(item);
    }
    else {
        this->hp = ceil(this->hp*0.85 - 0.001);
        this->exp = ceil(this->exp*0.85 - 0.001);
    }
    BaseItem* sth = bag->get();
    if (sth != nullptr) sth->use(this,nullptr);
    return false;
}
bool Watson::meet(RobotC* robotC)
{
    // TODO: Xử lý trao đổi khi gặp robot C
    if (hp%2 == 0) {
        BaseItem* temp = bag->get(PASSING_CARD);
        if (temp != nullptr) {
            temp->use(this, robotC);
            BaseItem* item = robotC->getItem();
            bag->insert(item);
            BaseItem* sth = bag->get();
            if (sth != nullptr) sth->use(this,nullptr);
            return false;
        }
    }
    BaseItem* item = robotC->getItem();
    bag->insert(item);

    BaseItem* sth = bag->get();
    if (sth != nullptr) sth->use(this,nullptr);
    return false;
}

bool Watson::meet(Sherlock* sherlock)
{
    // TODO: Xử lý trao đổi khi gặp Sherlock
    if (!(sherlock->getBag()->checkItem(PASSING_CARD) && bag->checkItem(EXCEMPTION_CARD))) return false;
    BaseItem* temp_sherItem = sherlock->getBag()->get(PASSING_CARD);
    BaseItem* temp_watsonItem = bag->get(EXCEMPTION_CARD);
    if (temp_sherItem != NULL && temp_watsonItem != NULL)
    {
        while (temp_sherItem != NULL && temp_watsonItem != NULL)
        {
            bag->insert(temp_sherItem);
            sherlock->getBag()->insert(temp_watsonItem);
            temp_sherItem = sherlock->getBag()->get(PASSING_CARD);
            temp_watsonItem = bag->get(EXCEMPTION_CARD);
        }
    }
    return false;
}

//  Class Map

Map::Map(int num_rows, int num_cols, int num_walls, Position* array_walls, int num_fake_walls, Position* array_fake_walls)
    : num_rows(num_rows), num_cols(num_cols)
{  
    // if (num_rows == 0 || num_cols == 0) return;
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

ElementType Map::getElementType(int i, int j) const
{
    return (map[i][j])->getType();
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
            if(watson->getexp() > ((r*257 + c*139 + 89)% 900 + 1)) return true;
            else return false;
        }
        else return false;
    }
    return true;
}

// Class Criminal

Criminal::Criminal(int index, const Position &init_pos, Map* map, Sherlock* sherlock, Watson* watson)
    : Character(index, init_pos, map, "Criminal")
{
    this->sherlock = sherlock;
    this->watson = watson;
    move_count = 0;
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
        if (map->isValid(U,this)) return U;
    }
    else if (temp == distance_L) {
        if (map->isValid(L,this)) return L;
    }
    else if (temp == distance_D) {
        if (map->isValid(D,this)) return D;
    }
    else if (temp == distance_R) {
        if (map->isValid(R,this)) return R;
    }
    return Position::npos;
}
void Criminal::move()
{
    prev_pos = pos;
    Position next_position = getNextPosition();
    move_count++;
    if(next_position.isEqual(Position::npos)) return;
    pos = next_position;
}

int Criminal::getexp() const{ return 0;}

int Criminal::gethp() const { return 0;}

void Criminal::sethp(int hp) {}
void Criminal::setexp(int exp) {}

string Criminal::str() const
{
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}

MovingObjectType Criminal::getObjectType() const
{
    return CRIMINAL;
}

Position Criminal::getPrevPosition() const
{
    return prev_pos;
}

int Criminal::getMoveCount() const
{
    return move_count;
}

// Class ArrayMovingObject

ArrayMovingObject::ArrayMovingObject(int capacity)
    : capacity(capacity)
{
    count = 0;
    arr_mv_objs = new MovingObject*[capacity]();
    for (int i = 0; i < capacity; i++) {
        arr_mv_objs[i] = NULL;
    }
}

ArrayMovingObject::~ArrayMovingObject()
{
    if (arr_mv_objs != nullptr) {
        for (int i = 3; i < count; i++) {
            delete arr_mv_objs[i];
        }
        delete[] arr_mv_objs;
    }
}

bool ArrayMovingObject::isFull() const
{
    return count == capacity;
}

bool ArrayMovingObject::add(MovingObject* mv_obj)
{
    if (isFull()) {
        return false;
    }
    for (int i = 0; i < capacity; i++) {
        if (arr_mv_objs[i] == NULL) {
            arr_mv_objs[i] = mv_obj;
            count++;
            return true;
        }
    }
    return true;
}

MovingObject* ArrayMovingObject::get(int index) const
{
    return arr_mv_objs[index];
}

int ArrayMovingObject::size() const
{
    return count;
}

bool ArrayMovingObject::checkMeet(int index) const
{
    MovingObject* mv_obj_1 = this->get(index);
    for (int i = 0; i < count; i++) {
        if (i == index) continue;
        MovingObject* mv_obj_2 = this->get(i);
        if (mv_obj_1->getCurrentPosition().isEqual(mv_obj_2->getCurrentPosition())) {
            if (mv_obj_1->getObjectType() == SHERLOCK) {
                Sherlock* sherlock = dynamic_cast <Sherlock*> (mv_obj_1);
                if (mv_obj_2->getObjectType() == WATSON) {
                    Watson* watson = dynamic_cast <Watson*> (mv_obj_2);
                    sherlock->meet(watson);
                    // return false;
                }
                if (mv_obj_2->getObjectType() == ROBOT) {
                    Robot* robot = dynamic_cast <Robot*> (mv_obj_2);
                    RobotType type = robot->getRobotType();
                    if (type == C) {
                        RobotC* robotC = dynamic_cast <RobotC*> (mv_obj_2);
                        if (sherlock->meet(robotC)) return true;
                        // return true;
                    }
                    if (type == S) {
                        RobotS* robotS = dynamic_cast <RobotS*> (mv_obj_2);
                        sherlock->meet(robotS);
                        // return false;
                    }
                    if (type == W) {
                        RobotW* robotW = dynamic_cast <RobotW*> (mv_obj_2);
                        sherlock->meet(robotW);
                        // return false;
                    }
                    if (type == SW) {
                        RobotSW* robotSW = dynamic_cast <RobotSW*> (mv_obj_2);
                        sherlock->meet(robotSW);
                        // return false;
                    }
                        
                }
                if (mv_obj_2->getObjectType() == CRIMINAL) return true;

            }
            if (mv_obj_1->getObjectType() == WATSON) {
                Watson* watson = dynamic_cast <Watson*> (mv_obj_1);
                if (mv_obj_2->getObjectType() == ROBOT) {
                    Robot* robot = dynamic_cast <Robot*> (mv_obj_2);
                    RobotType type = robot->getRobotType();
                    if (type == C) {
                        RobotC* robotC = dynamic_cast <RobotC*> (mv_obj_2);
                        watson->meet(robotC);
                        // return false;
                    }
                    if (type == S) {
                        RobotS* robotS = dynamic_cast <RobotS*> (mv_obj_2);
                        watson->meet(robotS);
                        // return false;
                    }
                    if (type == W) {
                        RobotW* robotW = dynamic_cast <RobotW*> (mv_obj_2);
                        watson->meet(robotW);
                        // return false;
                    }
                    if (type == SW) {
                        RobotSW* robotSW = dynamic_cast <RobotSW*> (mv_obj_2);
                        watson->meet(robotSW);
                        // return false;
                    }
                }
                if (mv_obj_2->getObjectType() == CRIMINAL) return true;
                if (mv_obj_2->getObjectType() == SHERLOCK) {
                    Sherlock* sherlock = dynamic_cast <Sherlock*> (mv_obj_2);
                    watson->meet(sherlock);
                    // return false;
                }
            }
            if (mv_obj_1->getObjectType() == CRIMINAL) {
                if (mv_obj_2->getObjectType() == ROBOT) {
                    // return false;
                }
                if (mv_obj_2->getObjectType() == SHERLOCK) return true;
                if (mv_obj_2->getObjectType() == WATSON) return true;
            }
            if (mv_obj_1->getObjectType() == ROBOT) {
                Robot* robot = dynamic_cast <Robot*> (mv_obj_1);
                if (mv_obj_2->getObjectType() == ROBOT) {
                    //return false;
                }
                if (mv_obj_2->getObjectType() == SHERLOCK) {
                    Sherlock* sherlock = dynamic_cast <Sherlock*> (mv_obj_2);
                    RobotType type = robot->getRobotType();
                    if (type == C) {
                        RobotC* robotC = dynamic_cast <RobotC*> (mv_obj_1);
                        if (sherlock->meet(robotC)) return true;
                        // return true;
                    }
                    if (type == S) {
                        RobotS* robotS = dynamic_cast <RobotS*> (mv_obj_1);
                        sherlock->meet(robotS);
                        // return false;
                    }
                    if (type == W) {
                        RobotW* robotW = dynamic_cast <RobotW*> (mv_obj_1);
                        sherlock->meet(robotW);
                        // return false;
                    }
                    if (type == SW) {
                        RobotSW* robotSW = dynamic_cast <RobotSW*> (mv_obj_1);
                        sherlock->meet(robotSW);
                        // return false;
                    }
                }
                if (mv_obj_2->getObjectType() == WATSON) {
                    Watson* watson = dynamic_cast <Watson*> (mv_obj_2);
                    RobotType type = robot->getRobotType();
                    if (type == C) {
                        RobotC* robotC = dynamic_cast <RobotC*> (mv_obj_1);
                        watson->meet(robotC);
                        // return true;
                    }
                    if (type == S) {
                        RobotS* robotS = dynamic_cast <RobotS*> (mv_obj_1);
                        watson->meet(robotS);
                        // return false;
                    }
                    if (type == W) {
                        RobotW* robotW = dynamic_cast <RobotW*> (mv_obj_1);
                        watson->meet(robotW);
                        // return false;
                    }
                    if (type == SW) {
                        RobotSW* robotSW = dynamic_cast <RobotSW*> (mv_obj_1);
                        watson->meet(robotSW);
                        // return false;
                    }
                }
                if (mv_obj_2->getObjectType() == CRIMINAL) { 
                    //return false;
                }
            }
        }
    }
    return false;
}

string ArrayMovingObject::str() const
{
    string res = "ArrayMovingObject[count=" + to_string(count) + ";capacity=" + to_string(capacity);
    for(int i = 0; i < capacity; i++) {
        if (arr_mv_objs[i]) res += ";" + arr_mv_objs[i]->str();
    }
    if (count == 0) res += ";";
    res += "]";
    return res;
}

//  Class Configuration

Configuration::Configuration(const string & filepath) 
{
    string line;
    ifstream fileinput(filepath);
    if (!fileinput.is_open()) {
        return;
    }
    while(!fileinput.eof()){
        while(getline(fileinput, line)){
            int equal_sign_pos = line.find('=');
            int comma_pos = line.find(',');
            string key = line.substr(0,equal_sign_pos);
            if (key == "MAP_NUM_ROWS"){
                string value = line.substr(equal_sign_pos+1);
                map_num_rows = stoi(value);
            }
            else if (key == "MAP_NUM_COLS"){
                string value = line.substr(equal_sign_pos+1);
                map_num_cols = stoi(value);
            }
            else if (key == "MAX_NUM_MOVING_OBJECTS"){
                string value = line.substr(equal_sign_pos+1);
                max_num_moving_objects = stoi(value);
            }
            else if (key == "SHERLOCK_MOVING_RULE"){
                string value = line.substr(equal_sign_pos+1);
                sherlock_moving_rule = value;
            }
            else if (key == "SHERLOCK_INIT_HP"){
                string value = line.substr(equal_sign_pos+1);
                sherlock_init_hp = stoi(value);
            }
            else if (key == "SHERLOCK_INIT_EXP"){
                string value = line.substr(equal_sign_pos+1);
                sherlock_init_exp = stoi(value);
            }
            if (key == "WATSON_MOVING_RULE"){
                string value = line.substr(equal_sign_pos+1);
                watson_moving_rule = value;
            }
            else if (key == "WATSON_INIT_HP"){
                string value = line.substr(equal_sign_pos+1);
                watson_init_hp = stoi(value);
            }
            else if (key == "WATSON_INIT_EXP"){
                string value = line.substr(equal_sign_pos+1);
                watson_init_exp = stoi(value);
            }
            else if (key == "NUM_STEPS"){
                string value = line.substr(equal_sign_pos+1);
                num_steps = stoi(value);
            }
            else if (key == "ARRAY_WALLS"){
                int count = 0;
                size_t pos1 = 0, pos2 = 0;
                while ((pos1 = line.find(',', pos1)) != string::npos) {
                    count++;
                    pos1++;
                }
                num_walls = count;
                if (count == 0) arr_walls = nullptr;
                else {
                    arr_walls = new Position[num_walls];
                    for (int i=0; i < count; i++) {
                        size_t open_pos = line.find('(',pos2);
                        size_t comma_pos = line.find(',',open_pos);
                        size_t close_pos = line.find(')',comma_pos);
                        string value1 = line.substr(open_pos + 1, comma_pos - open_pos - 1);
                        string value2 = line.substr(comma_pos + 1, close_pos - comma_pos - 1);
                        arr_walls[i] = Position(stoi(value1),stoi(value2));
                        pos2 = close_pos+1;
                    }    
                }
                
            }
            else if (key == "ARRAY_FAKE_WALLS"){
                int count = 0;
                size_t pos1 = 0, pos2 = 0;
                while ((pos1 = line.find(',', pos1)) != string::npos) {
                    count++;
                    pos1++;
                }
                num_fake_walls = count;
                if (count == 0) arr_fake_walls = nullptr;
                else {
                    arr_fake_walls = new Position[num_fake_walls];
                    for (int i=0;i < count; i++) {
                        size_t open_pos = line.find('(',pos2);
                        size_t comma_pos = line.find(',',open_pos);
                        size_t close_pos = line.find(')',comma_pos);
                        string value1 = line.substr(open_pos + 1, comma_pos - open_pos - 1);
                        string value2 = line.substr(comma_pos + 1, close_pos - comma_pos - 1);
                        arr_fake_walls[i] = Position(stoi(value1),stoi(value2));
                        pos2 = close_pos+1;
                    }
                }
                
            }
            else if (key=="SHERLOCK_INIT_POS"){
                size_t pos2=0;
                size_t open_pos=line.find('(',pos2);
                size_t comma_pos=line.find(',',open_pos);
                size_t close_pos=line.find(')',comma_pos);
                string value1 = line.substr(open_pos + 1, comma_pos - open_pos - 1);
                string value2 = line.substr(comma_pos + 1, close_pos - comma_pos - 1);
                sherlock_init_pos = Position(stoi(value1),stoi(value2));
            }
            else if (key=="WATSON_INIT_POS"){
                size_t pos2=0;
                size_t open_pos=line.find('(',pos2);
                size_t comma_pos=line.find(',',open_pos);
                size_t close_pos=line.find(')',comma_pos);
                string value1 = line.substr(open_pos + 1, comma_pos - open_pos - 1);
                string value2 = line.substr(comma_pos + 1, close_pos - comma_pos - 1);
                watson_init_pos = Position(stoi(value1),stoi(value2));
            }
            else if (key=="CRIMINAL_INIT_POS"){
                size_t pos2=0;
                size_t open_pos=line.find('(',pos2);
                size_t comma_pos=line.find(',',open_pos);
                size_t close_pos=line.find(')',comma_pos);
                string value1 = line.substr(open_pos + 1, comma_pos - open_pos - 1);
                string value2 = line.substr(comma_pos + 1, close_pos - comma_pos - 1);
                criminal_init_pos = Position(stoi(value1),stoi(value2));
            }
       }   
    }
}

Configuration::~Configuration()
{
    if (arr_fake_walls != nullptr) delete[] arr_fake_walls;
    if (arr_walls != nullptr) delete[] arr_walls;
}

string Configuration::str() const
{
    string arr_walls_string = "[";
    if (arr_walls != nullptr) {
        for (int i = 0; i < num_walls; i++) {
            arr_walls_string += arr_walls[i].str();
            if (i != num_walls - 1) arr_walls_string += ";";
        }
    }
    arr_walls_string += "]";

    string arr_fake_walls_string = "[";
    if (arr_fake_walls != nullptr) {
        for (int i = 0; i < num_fake_walls; i++) {
            arr_fake_walls_string += arr_fake_walls[i].str();
            if (i != num_fake_walls - 1) arr_fake_walls_string += ";";
        }
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

//  Class Robot

RobotType Robot::getRobotType() 
{
    return robot_type;
}

Robot* Robot::createRobot(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson)
{
    if (criminal->getMoveCount() == 3) return new RobotC(index, criminal->getPrevPosition(), map, criminal);

    if (criminal->getMoveCount()%3 == 0) {
        int criminal_sherlock_distance = ManhattanDistance(criminal->getPrevPosition(), sherlock->getCurrentPosition());
        int criminal_watson_distance =  ManhattanDistance(criminal->getPrevPosition(), watson->getCurrentPosition());
        if (criminal_sherlock_distance < criminal_watson_distance) {
            return new RobotS(index, criminal->getPrevPosition(), map, criminal, sherlock);
        }
        else if (criminal_sherlock_distance > criminal_watson_distance) {
            return new RobotW(index, criminal->getPrevPosition(), map, criminal, watson);
        }
        else {
            return new RobotSW(index, criminal->getPrevPosition(), map, criminal, sherlock, watson);
        }
    }
    return NULL;
}

MovingObjectType Robot::getObjectType() const
{
    return ROBOT;
}

BaseItem* Robot::getItem() {
    return item;
}

//  Class RobotC

RobotC::RobotC(int index, const Position &init_pos, Map* map, Criminal* criminal)
    : Robot(index, init_pos, map, criminal, "RobotC")
{
    robot_type = C;
}

RobotC::~RobotC()
{
    delete item;
}

int RobotC::getDistance(Sherlock* sherlock)
{
    return ManhattanDistance(pos,sherlock->getCurrentPosition());
}

int RobotC::getDistance(Watson* watson)
{
    return ManhattanDistance(pos,watson->getCurrentPosition());
}

Position RobotC::getNextPosition()
{
    Position next_position = pos;
    next_position = criminal->getPrevPosition();
    if (map->isValid(next_position, this)) return next_position;
    return Position::npos;
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

//  Class RobotW

RobotW::RobotW(int index, const Position &init_pos, Map* map, Criminal* criminal, Watson* watson)
    : Robot(index, init_pos, map, criminal, "RobotW"), watson(watson)
{
    robot_type = W;
}

RobotW::~RobotW()
{
    delete item;
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
        if (map->isValid(U,this)) return U;
    }
    else if (temp == distance_R) {
        if (map->isValid(R,this)) return R;
    }
    else if (temp == distance_D) {
        if (map->isValid(D,this)) return D;
    }
    else if (temp == distance_L) {
        if (map->isValid(L,this)) return L;
    }
    return Position::npos;
}

void RobotW::move()
{
    Position next_position = getNextPosition();
    if (next_position.isEqual(Position::npos)) return;
    pos = next_position;
}

string RobotW::str() const
{
    return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(getDistance()) + "]";
}

int RobotW::getDistance() const
{
    return ManhattanDistance(pos,watson->getCurrentPosition());
}

//  Class RobotS
RobotS::RobotS(int index, const Position &init_pos, Map* map, Criminal* criminal, Sherlock* sherlock)
    : Robot(index, init_pos, map, criminal, "RobotS"), sherlock(sherlock)
{
    robot_type = S;
}

RobotS::~RobotS()
{
    delete item;
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
        if (map->isValid(U,this)) return U;
    }
    else if (temp == distance_R) {
        if (map->isValid(R,this)) return R;
    }
    else if (temp == distance_D) {
        if (map->isValid(D,this)) return D;
    }
    else if (temp == distance_L) {
        if (map->isValid(L,this)) return L;
    }
    return Position::npos;
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

int RobotS::getDistance() const
{
    return ManhattanDistance(pos,sherlock->getCurrentPosition());
}

//  Class RobotSW

RobotSW::RobotSW(int index, const Position &init_pos, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson)
    : Robot(index, init_pos, map, criminal, "RobotSW")
{
    robot_type = SW;
    this->sherlock = sherlock;
    this->watson = watson;
}

RobotSW::~RobotSW()
{
    delete item;
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
    
    if (temp == INT_MAX) return Position::npos;
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

//  Class BaseItem

BaseItem::BaseItem(){};

//  Class MagicBook

ItemType MagicBook::getItemType() const
{
    return MAGIC_BOOK;
}

string MagicBook::str() const
{
    return "MagicBook";
}

bool MagicBook::canUse(Character* obj, Robot* robot)
{
    if (obj->getObjectType() == SHERLOCK ||  obj->getObjectType() == WATSON) {
        if (obj->getexp() <= 350 && robot == nullptr) return true;
    }
    return false;
}

void MagicBook::use(Character* obj, Robot* robot)
{
    if (canUse(obj, robot)) {
        obj->setexp(ceil(obj->getexp()*1.25 - 0.00001));
    }
}

//  Class EnergyDrink

ItemType EnergyDrink::getItemType() const
{
    return ENERGY_DRINK;
}

string EnergyDrink::str() const
{
    return "EnergyDrink";
}

bool EnergyDrink::canUse(Character* obj, Robot* robot)
{
    if (obj->getObjectType() == SHERLOCK ||  obj->getObjectType() == WATSON) {
        if (obj->gethp() <= 100 && robot == nullptr) return true;
    }
    return false;
}

void EnergyDrink::use(Character* obj, Robot* robot)
{
    if (canUse(obj, robot)) {
        obj->sethp(ceil(obj->gethp()*1.2 - 0.00001));
    }
}

//  Class FirstAid

ItemType FirstAid::getItemType() const
{
    return FIRST_AID;
}

string FirstAid::str() const
{
    return "FirstAid";
}

bool FirstAid::canUse(Character* obj, Robot* robot)
{
    if (obj->getObjectType() == SHERLOCK ||  obj->getObjectType() == WATSON) {
        if ((obj->gethp() <= 100 || obj->getexp() <= 350) && robot==nullptr) return true;
    }
    return false;
}

void FirstAid::use(Character* obj, Robot* robot)
{
    if (canUse(obj, robot)) {
        obj->sethp(ceil(obj->gethp()*1.5 - 0.00001));
    }
}

//  Class ExcemptionCard
ItemType ExcemptionCard::getItemType() const
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
        if (obj->gethp()%2) return true;
    }
    return false;
}

void ExcemptionCard::use(Character* obj, Robot* robot)
{}

//  Class PassingCard
PassingCard::PassingCard(int i, int j)
{
    int t = (i*11+j)%4;
    if (t == 0) challenge = "RobotS";
    else if (t == 1) challenge = "RobotC";
    else if (t == 2) challenge = "RobotSW";
    else challenge = "all";
}

PassingCard::PassingCard(string in_challenge)
{
    challenge = in_challenge;
}

ItemType PassingCard::getItemType() const
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
        if (obj->gethp()%2 == 0) return true;
    }
    return false;
}

void PassingCard::use(Character* obj, Robot* robot)
{
    if (canUse(obj, robot)) {
        RobotType type = robot->getRobotType();
        if (challenge == "all") return;
        else {
            if (challenge == "RobotS") {
                if (type != S) obj->setexp(obj->getexp()-50);
            }
            else if (challenge == "RobotC") {
                if (type != C) obj->setexp(obj->getexp()-50);
            }
            else if (challenge == "RobotSW") {
                if (type != SW) obj->setexp(obj->getexp()-50);
            }
        }
    }
    if (obj->getexp() < 0) obj->setexp(0);
}

//  Class BaseBag

BaseBag::BaseBag(int capacity)
{
    this->capacity = capacity;
    size = 0;
    head = nullptr;
}

BaseBag::~BaseBag()
{
    while (head != nullptr)
    {
        Node* temp = head;
        head = head->next;
        // delete temp->item;
        delete temp;
    }
}

bool BaseBag::insert(BaseItem* item)
{
    if (item == nullptr) return false;
    Node* newNode = new Node(item, nullptr);
    if (this->size == 0) {
        // TODO: Adds Node with item at the head of the Linked List
        head = newNode;
        this->size++;
        return true;
    }
    else if (this->size < capacity) {
        newNode->next = head;
        head = newNode;
        this->size++;
        return true;
    }
    return false;
}

BaseItem* BaseBag::get(ItemType itemType)
{
    // Finds the item matches the itemType
    // Swap Nodes
    // Delete the head then update the head
    string arr_name[] = {"MagicBook", "EnergyDrink", "FirstAid", "ExcemptionCard", "PassingCard"};

    if (this->head == NULL)
        return NULL;

    // When Linked List has only 1 element
    if (this->head->next == NULL)
    {
        if (this->head->item->str() == arr_name[itemType])
        {
            BaseItem* temp = this->head->item;
            delete this->head;
            this->head = NULL;
            this->size--;
            return temp;
        }
        return NULL;
    }

    // If the item is the first element
    if (this->head->item->str() == arr_name[itemType])
    {
        BaseItem* res_item = this->head->item;
        Node* temp = this->head;
        this->head = this->head->next;
        delete temp;
        this->size--;
        return res_item;
    }

    Node* prev = NULL;
    Node* current = this->head;

    // Finding the item
    while (current != nullptr && current->item->str() != arr_name[itemType])
    {
        prev = current;
        current = current->next;
    }

    // Not found
    if (current == nullptr)
        return NULL;

    // Swapping nodes
    Node* temp = this->head;
    this->head = current;
    if (prev)
        prev->next = temp;
    Node* temp_next = current->next;
    current->next = temp->next;
    temp->next = temp_next;

    // Delete head and update head
    BaseItem* res_item = this->head->item;
    temp = this->head;
    this->head = this->head->next;
    delete temp;
    this->size--;

    return res_item;

}

BaseItem* BaseBag::get() {return nullptr;}

bool BaseBag::checkItem(ItemType itemType)
{
    for (int i = 0; i < size; i++) {
        Node* temp = head;
        if (temp->item->getItemType() == itemType) return true;
    }
    return false;
}

string BaseBag::str() const
{
    Node* current = this->head;
    string str = "Bag[count=" + to_string(size) + ";";
    while (current != nullptr)
    {
        str += current->item->str() + ",";
        current = current->next;
    }
    if (size == 0) str += "]";
    else str.back() = ']';
    return str;
}

//  Class SherlockBag
SherlockBag::SherlockBag(Sherlock* character)
    : BaseBag(13)
{
    this->sherlock = character;
}

BaseItem* SherlockBag::get()
{
    // Find the item that Sherlock can use and swap, and delete it from Linked List
    if (this->head == NULL)
        return NULL;

    // When Linked List has only 1 element
    if (this->head->next == NULL)
    {
        if (this->head->item->canUse(this->sherlock,nullptr))
        {
            BaseItem* temp = this->head->item;
            delete this->head;
            this->head = NULL;
            this->size--;
            return temp;
        }
        return NULL;
    }

    // If it's the first node
    if (this->head->item->canUse(this->sherlock,nullptr))
    {
        BaseItem* res_item = this->head->item;
        Node* temp = this->head;
        this->head = this->head->next;
        delete temp;
        this->size--;
        return res_item;
    }

    Node* prev = NULL;
    Node* current = this->head;

    // Finding the item
    while (current != nullptr && !current->item->canUse(this->sherlock,nullptr))
    {
        prev = current;
        current = current->next;
    }

    // Not found
    if (current == nullptr)
        return NULL;

    // Swapping nodes
    Node*temp = this->head;
    this->head = current;
    if (prev)
        prev->next = temp;
    Node* temp_next = current->next;
    current->next = temp->next;
    temp->next = temp_next;

    // Delete head and update head
    BaseItem* res_item = this->head->item;
    temp = this->head;
    this->head = this->head->next;
    delete temp;
    this->size--;

    return res_item;
}

//  Class WatsonBag

WatsonBag::WatsonBag(Watson* character)
    : BaseBag(15)
{
    this->watson = character;
}
BaseItem* WatsonBag::get()
{
    // Find the item that Watson can use and swap, and delete it from Linked List
    if (this->head == NULL)
        return NULL;

    // When Linked List has only 1 element
    if (this->head->next == NULL)
    {
        if (this->head->item->canUse(this->watson,nullptr))
        {
            BaseItem* temp = this->head->item;
            delete this->head;
            this->head = NULL;
            this->size--;
            return temp;
        }
        return NULL;
    }

    // If it's the first node
    if (this->head->item->canUse(this->watson,nullptr))
    {
        BaseItem* res_item = this->head->item;
        Node* temp = this->head;
        this->head = this->head->next;
        delete temp;
        this->size--;
        return res_item;
    }

    Node* prev = NULL;
    Node* current = this->head;

    // Finding the item
    while (current != nullptr && !current->item->canUse(this->watson,nullptr))
    {
        prev = current;
        current = current->next;
    }

    // Not found
    if (current == nullptr)
        return NULL;

    // Swapping nodes
    Node*temp = this->head;
    this->head = current;
    if (prev)
        prev->next = temp;
    Node* temp_next = current->next;
    current->next = temp->next;
    temp->next = temp_next;

    // Delete head and update head
    BaseItem* res_item = this->head->item;
    temp = this->head;
    this->head = this->head->next;
    delete temp;
    this->size--;

    return res_item;
}

//  Class Robot

Robot::Robot(int index, const Position &pos, Map* map, Criminal* criminal, const string &name)
    : MovingObject(index, pos, map, "Robot"), criminal(criminal)
{
    this->original_pos = pos;
    int i = pos.getRow(), j = pos.getCol();
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
        if (t == 0) item = new PassingCard(i,j);
        else if (t == 1) item = new PassingCard(i,j);
        else if (t == 2) item = new PassingCard(i,j);
        else if (t == 3) item = new PassingCard(i,j);
        break;
    }
    default:
        break;
    }
}

int Robot::getDistance() const
{
    return 0;
}

Position Robot::CriminalPos()
{
    return criminal->getCurrentPosition();
}

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
    stopChecker = false;
}

StudyPinkProgram::~StudyPinkProgram()
{
    delete map;
    delete config;
    delete arr_mv_objs;
    delete criminal;
    delete sherlock;
    delete watson;
}

bool StudyPinkProgram::isStop() const
{
    if (sherlock->gethp() == 1 || watson->gethp() == 1 
        || sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition())
        || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition())) 
        return true;
    return false;
}

void StudyPinkProgram::run(bool verbose)
{
    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        int roundSize = arr_mv_objs->size();   
                                    // && !stopChecker
        for (int i = 0; i < roundSize ; ++i)
        {
            arr_mv_objs->get(i)->move();
            bool stopChecker = arr_mv_objs->checkMeet(i);
            if (isStop() || stopChecker) {
                printStep(istep);
                printResult();
                return;
            }
            MovingObject* robot = nullptr;
            if (arr_mv_objs->get(i)->getObjectType() == MovingObjectType::CRIMINAL)
            {
                Criminal* criminal = dynamic_cast <Criminal*> (arr_mv_objs->get(i));
                if (!criminal->getPrevPosition().isEqual(criminal->getCurrentPosition()))
                    robot = Robot::createRobot(arr_mv_objs->size(), map, criminal, sherlock, watson);
            }
            if (robot != nullptr) {
                arr_mv_objs->add(robot);
                roundSize = arr_mv_objs->size();
            }
            if (verbose) printStep(istep);
            if (isStop() || stopChecker) {
                printStep(istep);
                printResult();
                return;
            }
        }
    }
    printResult();
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////