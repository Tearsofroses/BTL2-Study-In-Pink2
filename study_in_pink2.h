/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2023
* Author: Vu Van Tien
* Date: 02.02.2023
*/

//The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

class MovingObject;
class Position;
class Configuration;
class Map;

class Sherlock;
class Watson;
class Criminal;
class Robot;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;
enum MovingObjectType{SHERLOCK, WATSON, CRIMINAL, ROBOT };
enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };

class MapElement {
    friend class TestStudyInPink;
protected:
    ElementType type;
public:
    MapElement(ElementType in_type);
    virtual ~MapElement();

    virtual ElementType getType() const;
};

class Path : public MapElement {
    friend class TestStudyInPink;
public:
    Path();
};

class Wall : public MapElement {
    friend class TestStudyInPink;
public:
    Wall();
};

class FakeWall : public MapElement {
    friend class TestStudyInPink;
private:
    int req_exp;
public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map {
    friend class TestStudyInPink;
private:
    int num_rows, num_cols;
    MapElement*** map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position * array_walls, int num_fake_walls, Position * array_fake_walls);
    ~Map();

    int getNumRows() const;
    int getNumCols() const;
    ElementType getElementType(const Position &pos) const;
    ElementType getElementType(int in_r, int in_c) const;
    bool isValid(const Position &pos, MovingObject * mv_obj) const;
};

class Position {
    friend class TestStudyInPink;
private:
    int r, c;
public:
    static const Position npos;

    Position(int r=0, int c=0);
    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool isEqual(const Position & pos) const;
};

class MovingObject {
    friend class TestStudyInPink;
protected:
    int index;
    Position pos;
    Map* map;
    string name;

public:
    MovingObject(int index, const Position pos, Map * map, const string & name="");
    virtual ~MovingObject();

    Position getCurrentPosition() const;
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual MovingObjectType getObjectType() const = 0;
    int getIndex() const;
};

class Character : public MovingObject {
public:
    Character(int index, const Position pos, Map * map, const string &name="");

    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual int getHP() const = 0;
    virtual void setHP(int init_hp) = 0;
    virtual int getEXP() const = 0;
    virtual void setEXP(int init_exp) = 0;
    virtual string str() const = 0;
    virtual MovingObjectType getObjectType() const = 0;
    virtual void action(ArrayMovingObject* arr_mv_objs, SherlockBag*  sherlockbag, WatsonBag* watsonbag);
};

class Sherlock : public Character {
    friend class TestStudyInPink;
private:
    string moving_rule;
    int hp;
    int exp;
    int move_count = 0;

public:
    Sherlock(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    
    Position getNextPosition();
    void move();
    void setHP(int init_hp);
    void setEXP(int init_exp);
    string str() const;
    int getHP() const;
    int getEXP() const;
    MovingObjectType getObjectType() const;
    void action(ArrayMovingObject* arr_mv_objs, SherlockBag*  sherlockbag, WatsonBag* watsonbag) override;
};

class Watson : public Character {
    friend class TestStudyInPink;
private:
    string moving_rule;
    int hp;
    int exp;
    int move_count = 0;

public:
    Watson(int index, const string & moving_rule, const Position & init_pos, Map * map, int init_hp, int init_exp);
    
    Position getNextPosition();
    void move();
    string str() const;
    int getHP() const;
    int getEXP() const;
    void setEXP(int init_exp);
    void setHP(int init_hp);
    MovingObjectType getObjectType() const;
    void action(ArrayMovingObject* arr_mv_objs, SherlockBag*  sherlockbag, WatsonBag* watsonbag) override;
};


class Criminal : public Character {
    friend class TestStudyInPink;
private:
    Sherlock* sherlock;
    Watson* watson;
    int move_count = 0;

public:
    Criminal(int index, const Position & init_pos, Map * map, Sherlock * sherlock, Watson * watson);
    int getHP() const;
    int getEXP() const;
    void setEXP(int init_exp);
    void setHP(int init_hp);
    bool isCreatedRobotNext() const;
    string str() const;
    Position getNextPosition();
    MovingObjectType getObjectType() const;
    int getCount() const;
    void move();
    void create(ArrayMovingObject* arr_mv_objs, Sherlock* sherlock, Watson* watson);
};

class ArrayMovingObject {
    friend class TestStudyInPink;
private:
    MovingObject** arr_mv_objs;
    int count = 0;
    int capacity;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject() ;
    
    bool isFull() const;
    bool add(MovingObject* mv_obj);
    MovingObject* get(int index) const;
    int MovingObjectCount() const;
    int size() const;
    string str() const;
};

/*class Configuration {
private:
    int map_num_rows = 0, map_num_cols = 0;
    int max_num_moving_objects = 0;
    int num_walls = 0;
    Position* arr_walls = nullptr;
    int num_fake_walls = 0;
    Position* arr_fake_walls = nullptr;
    string sherlock_moving_rule = "";
    Position sherlock_init_pos = Position(-1,-1);
    int sherlock_init_hp = 0;
    int sherlock_init_exp = 0;
    string watson_moving_rule = "";
    Position watson_init_pos = Position(-1,-1);
    int watson_init_hp = 0;
    int watson_init_exp = 0;
    Position criminal_init_pos = Position(-1,-1);
    int num_steps = 0;
public:
    friend class StudyPinkProgram;
    friend class TestStudyInPink;
    Configuration(const string & filepath);
    ~Configuration();
    string str() const;
};*/

class Configuration
{
private:
    int map_num_rows;
    int map_num_cols = 0;
    int max_num_moving_objects = 0;
    int num_walls = 0;
    Position *arr_walls = NULL;
    int num_fake_walls = 0;
    Position *arr_fake_walls = NULL;
    string sherlock_moving_rule = "";
    Position sherlock_init_pos = Position(-1, -1);
    int sherlock_init_hp = 0;
    int sherlock_init_exp = 0;
    string watson_moving_rule = "";
    Position watson_init_pos = Position(-1, -1);
    int watson_init_hp = 0;
    int watson_init_exp = 0;
    Position criminal_init_pos = Position(-1, -1);
    int num_steps;

public:
    friend class TestStudyPink;

    // Constructor & Destructor
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;

    // Getter
    int getNumRows() const { return this->map_num_rows; }
    int getNumCols() const { return this->map_num_cols; }
    int getMaxNumMovingObjects() const { return this->max_num_moving_objects; }
    int getNumWalls() const { return this->num_walls; }
    Position *getArrWalls() const { return this->arr_walls; }
    int getNumFakeWalls() const { return this->num_fake_walls; }
    Position *getArrFakeWalls() const { return this->arr_fake_walls; }
    string getSherlockMovingRule() const { return this->sherlock_moving_rule; }
    Position getSherlockInitPos() const { return this->sherlock_init_pos; }
    int getSherlockInitHp() const { return this->sherlock_init_hp; }
    int getSherlockInitExp() const { return this->sherlock_init_exp; }
    string getWatsonMovingRule() const { return this->watson_moving_rule; }
    Position getWatsonInitPos() const { return this->watson_init_pos; }
    int getWatsonInitHp() const { return this->watson_init_hp; }
    int getWatsonInitExp() const { return this->watson_init_exp; }
    Position getCriminalInitPos() const { return this->criminal_init_pos; }
    int getNumSteps() const { return this->num_steps; }
};

// Robot, BaseItem, BaseBag,...

class Robot : public MovingObject {
protected:
    Criminal* criminal;
    BaseItem* item;
    RobotType robot_type;

public:
    Robot(int index, const Position &pos, Map* map, RobotType robot_type, Criminal* criminal, const string &name = "");
    ~Robot();

    MovingObjectType getObjectType() const;
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual RobotType getType() const = 0;
    virtual int getDistance() const = 0;
    BaseItem* getItem() const;
};

class RobotC : public Robot {
public:
    RobotC(int index, const Position &init_pos, Map* map, Criminal* criminal);

    int getDistance(Sherlock* sherlock);
    int getDistance(Watson* watson);
    Position getNextPosition();
    void move();
    string str() const;
    int getDistance() const;
    RobotType getType() const;
};

class RobotW : public Robot {
private:
    Watson* watson;

public:
    RobotW(int index, const Position &init_pos, Map* map, Criminal* criminal, Watson* watson);

    Position getNextPosition();
    void move();
    string str() const;
    RobotType getType() const;
    int getDistance() const;
};

class RobotS : public Robot {
private:
    Sherlock* sherlock;

public:
    RobotS(int index, const Position &init_pos, Map* map, Criminal* criminal, Sherlock* sherlock);

    Position getNextPosition();
    void move();
    string str() const;
    RobotType getType() const;
    int getDistance() const;
};

class RobotSW : public Robot {
private:
    Sherlock* sherlock;
    Watson* watson;

public:
    RobotSW(int index, const Position &init_pos, Map* map, Criminal* criminal, Sherlock* sherlock, Watson *watson);

    Position getNextPosition();
    string str() const;
    RobotType getType() const;
    int getDistance() const;
    void move();
};

class BaseItem {
protected:
    ItemType type;
    string name;
public:
    BaseItem(ItemType in_type);
    virtual bool canUse(Character* obj, Robot* robot) = 0;
    virtual void use(Character* obj, Robot* robot) = 0;
    virtual ItemType getType() const = 0;
    virtual string str() const = 0;
    ~BaseItem() {};
};

class MagicBook : public BaseItem {
public:
    MagicBook();
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    ItemType getType() const;
    string str() const;
};

class EnergyDrink : public BaseItem {
public:
    EnergyDrink();
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    ItemType getType() const;
    string str() const;
};

class FirstAid : public BaseItem {
public:
    FirstAid();
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const;
    string str() const;
};

class ExcemptionCard : public BaseItem {
public:
    ExcemptionCard();
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    ItemType getType() const;
    string str() const;
};

class PassingCard : public BaseItem {
private:
    string challenge;

public:
    //PassingCard(int i, int j);
    PassingCard(string challenge);
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    ItemType getType() const;
    string str() const;
};

/*class BaseBag {
    friend class TestStudyInPink;
protected:
    class Node {
    public:
        BaseItem* item;
        Node* next;

    public:
        Node() : item(nullptr), next(nullptr) {}
        Node(BaseItem* item, Node* next = nullptr) : item(item), next(next) {}
        ~Node() {}
    };

    class LinkedList {
    private:
        Node* head;
    public:
        LinkedList();
        ~LinkedList();
        void insert(BaseItem* item);
        Node* getHead();
        void deleteNode(Node* node);
        void swapNode(Node* node);
        void deleteHead();

    };

protected:
    int size;
    int capacity;
    Node* head;
public:
    BaseBag(int capacity);
    virtual ~BaseBag();
    virtual bool insert(BaseItem* item);
    virtual BaseItem* get() = 0;
    virtual BaseItem* get(ItemType itemType);
    virtual string str() const;
};

class SherlockBag : public BaseBag {
private:
    Sherlock* sherlock;

public:
    SherlockBag(Sherlock* character);
    BaseItem* get();
};

class WatsonBag : public BaseBag {
private:
    Watson* watson;

public:
    WatsonBag(Watson* character);
    BaseItem* get();
};*/

class Node {
    public:
        BaseItem* item;
        Node* next;

        Node();
        Node(BaseItem* item);
        ~Node();
};

class BaseBag {
    friend class TestStudyInPink;
protected:
    int capacity;
    int count = 0;
    Character* obj;
    Node* head = NULL;
public:
    BaseBag(Character* obj);
    ~BaseBag();

    virtual bool insert(BaseItem* item);
    virtual BaseItem* get();
    virtual BaseItem* get(ItemType itemType);
    Character* getObj();
    virtual string str() const;

    void deleteNode(Node* node);
    void swapNode(Node* node);
    void deleteHead();
};

class SherlockBag : public BaseBag {
private:
    Sherlock* sherlock;
public:
    SherlockBag(Sherlock* sherlock);
};

class WatsonBag : public BaseBag {
private:
    Watson* watson;
public:
    WatsonBag(Watson* watson);
};



class StudyPinkProgram {
    friend class TestStudyInPink;
private:
    Configuration* config;
    
    Sherlock* sherlock;
    Watson* watson;
    Criminal* criminal;
    
    Map* map;
    ArrayMovingObject* arr_mv_objs;
    SherlockBag* sherlockbag;
    WatsonBag* watsonbag;

public:
    StudyPinkProgram(const string & config_file_path);
    ~StudyPinkProgram();
    bool isStop() const;
    void printResult() const;

    void printStep(int si) const;

    void run(bool verbose);
};








// Supporting functions declartions

int ManhattanDistance(const Position &pos1, const Position &pos2);




////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */









