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
    Map * map;
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
};

class Sherlock : public Character {
    friend class TestStudyInPink;
private:
    string moving_rule;
    int hp;
    int exp;
    int move_count = 0;
    BaseBag* Bag;

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
    void meet(Robot* robot);
    void meet(Watson* watson);
};

class Watson : public Character {
    friend class TestStudyInPink;
private:
    string moving_rule;
    int hp;
    int exp;
    BaseBag* Bag;
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
    void meet(Robot* robot);
    void meet(Sherlock* sherlock);
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
    int size() const; // return current number of elements in the array
    string str() const;
};

class Configuration {
    friend class StudyPinkProgram;
    friend class TestStudyInPink;
private:
    int map_num_rows, map_num_cols;
    int max_num_moving_objects;
    int num_walls = 0;
    Position* arr_walls;
    int num_fake_walls = 0;
    Position* arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;
public:
    Configuration(const string & filepath);
    ~Configuration();
    
    string str() const;
};

// Robot, BaseItem, BaseBag,...

class Robot : public MovingObject {
protected:
    Criminal* criminal;
    BaseItem* item;
    RobotType robot_type;

public:
    Robot(int index, const Position &pos, Map* map, RobotType robot_type, Criminal* criminal, const string &name = "");

    static Robot* create(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson);
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
        BaseItem* data;
        Node* next;

        Node();
        Node(BaseItem* data);
        ~Node();
};

class Linkedlist {
private:
    Node* head;
public:
    Linkedlist();
    ~Linkedlist();
    void insert(BaseItem* item);
    Node* getHead();
    void deleteNode(Node* node);
    void swapNode(Node* node);
    void deleteHead();
    int countItem();
    string printList();
};

class BaseBag {
    friend class TestStudyInPink;
private:
    Character* obj;
public:
    BaseBag(Character* obj);
    virtual bool insert(BaseItem* item) = 0;
    virtual BaseItem* get() = 0;
    virtual BaseItem* get(ItemType itemType) = 0;
    virtual string str() const = 0;
    Character* getObj();
    virtual BaseItem* find(ItemType itemtype) = 0;
};

class SherlockBag : public BaseBag {
private:
    int maximum = 13;
    Linkedlist* sherlockbag = new Linkedlist();

public:
    SherlockBag(Sherlock* character);
    ~SherlockBag();
    bool insert(BaseItem* item);
    BaseItem* get();
    BaseItem* get(ItemType itemtype);
    string str() const;
    BaseItem* find(ItemType itemtype);
};

class WatsonBag : public BaseBag {
private:
    int maximum = 15;
    Linkedlist* watsonbag = new Linkedlist();

public:
    WatsonBag(Watson* character);
    ~WatsonBag();
    bool insert(BaseItem* item);
    BaseItem* get();
    BaseItem* get(ItemType itemtype);
    string str() const;
    BaseItem* find(ItemType itemtype);
};



class StudyPinkProgram {
private:
    Configuration* config;
    
    Sherlock* sherlock;
    Watson* watson;
    Criminal* criminal;
    
    Map* map;
    ArrayMovingObject* arr_mv_objs;


public:
    StudyPinkProgram(const string & config_file_path);

    bool isStop() const;
    void printMap(ofstream &OUTPUT) const;
    void printResult() const {
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

    void printStep(int si) const {
        cout << "Step: " << setw(4) << setfill('0') << si
            << "--"
            << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose) {
        // Note: This is a sample code. You can change the implementation as you like.
        // TODO
        for (int istep = 0; istep < config->num_steps; ++istep) {
            for (int i = 0; i < arr_mv_objs->size(); ++i) {
                arr_mv_objs->get(i)->move();
                if (isStop()) {
                    printStep(istep);
                    break;
                }
                if (verbose) {
                    printStep(istep);
                }
            }
        }
        printResult();
    }
    void run(ofstream &OUTPUT);
    ~StudyPinkProgram();
};








// Supporting functions declartions

int ManhattanDistance(const Position &pos1, const Position &pos2);




////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */









