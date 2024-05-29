/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"
////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
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

class Character;
class Watson;
class Sherlock;

class TestStudyInPink;

enum ItemType { MAGIC_BOOK, ENERGY_DRINK, FIRST_AID, EXCEMPTION_CARD, PASSING_CARD };
enum ElementType { PATH, WALL, FAKE_WALL };
enum RobotType { C=0, S, W, SW };
enum MovingObjectType { SHERLOCK, WATSON, CRIMINAL, ROBOT };

class MapElement
{
    friend class TestStudyInPink;

protected:
    ElementType type;

public:
    MapElement(ElementType in_type);
    virtual ~MapElement();
    virtual ElementType getType() const;
};

class Path : public MapElement
{
    friend class TestStudyInPink;

public:
    Path();
};

class Wall : public MapElement
{
    friend class TestStudyInPink;

public:
    Wall();
};

class FakeWall : public MapElement
{
    friend class TestStudyInPink;

private:
    int req_exp;

public:
    FakeWall(int in_req_exp);
    int getReqExp() const;
};

class Map
{
    friend class TestStudyInPink;

private:
    int num_rows, num_cols;
    MapElement*** map;

public:
    Map(int num_rows, int num_cols, int num_walls, Position* array_walls, int num_fake_walls, Position* array_fake_walls);
    ~Map();
    bool isValid(const Position & pos, MovingObject* mv_obj) const;
    ElementType getElementType(int i, int j) const;
};

class Position
{
    friend class TestStudyInPink;

private:
    int r, c;

public:
    static const Position npos;

    Position(int r = 0, int c = 0);
    Position(const string & str_pos);

    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);

    string str() const;
    bool isEqual(int in_r, int in_c) const;
    bool isEqual(Position pos) const;
};

class MovingObject
{
    friend class TestStudyInPink;

protected:
    int index;
    Position pos;
    Map* map;
    string name;

public:
    MovingObject(int index, const Position pos, Map* map, const string &name = "");
    virtual ~MovingObject();
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual MovingObjectType getObjectType() const = 0;
};

class Character : public MovingObject
{
    friend class TestStudyInPink;

public:
    Character(int index, const Position pos, Map* map, const string &name = "");
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual MovingObjectType getObjectType() const = 0;
    virtual int gethp() const = 0;
    virtual int getexp() const = 0;
    virtual void sethp(int hp) = 0;
    virtual void setexp(int exp) = 0;
};

class Sherlock : public Character
{
    friend class TestStudyInPink;

private:
    int hp, exp;
    string moving_rule;
    int index_moving_rule;
    BaseBag* bag;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map* map, int init_hp, int init_exp);
    Position getNextPosition();
    void move();
    string str() const;
    MovingObjectType getObjectType() const;
    BaseBag* getBag() const;
    int gethp() const;
    int getexp() const;
    void sethp(int hp);
    void setexp(int exp);
    bool meet(RobotS* robotS);
    bool meet(RobotW* robotW);
    bool meet(RobotSW* robotSW);
    bool meet(RobotC* robotC);
    bool meet(Watson* watson);
    ~Sherlock();
};

class Watson : public Character
{
    friend class TestStudyInPink;

private:
    int hp, exp;
    string moving_rule;
    int index_moving_rule;
    BaseBag* bag;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map* map, int init_hp, int init_exp);
    Position getNextPosition();
    void move();
    string str() const;
    MovingObjectType getObjectType() const;
    BaseBag* getBag() const;
    int gethp() const;
    int getexp() const;
    void sethp(int hp);
    void setexp(int exp);
    bool meet(RobotS* robotS);
    bool meet(RobotW* robotW);
    bool meet(RobotSW* robotSW);
    bool meet(RobotC* robotC);
    bool meet(Sherlock* sherlock);
    ~Watson();
};

class Criminal : public Character
{
    friend class TestStudyInPink;

private:
    Sherlock* sherlock;
    Watson* watson;
    int move_count;
    Position prev_pos = Position::npos;

public:
    Criminal(int index, const Position &init_pos, Map* map, Sherlock* sherlock, Watson* watson);
    Position getNextPosition();
    void move();
    string str() const;
    
    MovingObjectType getObjectType() const;
    int getMoveCount() const;
    Position getPrevPosition() const;
    int gethp() const;
    int getexp() const;
    void sethp(int hp);
    void setexp(int exp);
};

class ArrayMovingObject
{
    friend class TestStudyInPink;

private:
    // TODO
    MovingObject** arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();
    bool isFull() const;
    bool add(MovingObject* mv_obj);
    string str() const;
    
    bool checkMeet(int index) const;
    MovingObject* get(int index) const;
    int size() const;
};

class Configuration
{
    friend class TestStudyInPink;
    friend class StudyPinkProgram;

private:
    int map_num_rows = 0;
    int map_num_cols = 0;
    int max_num_moving_objects = 0;
    int num_walls = 0;
    Position* arr_walls = nullptr;
    int num_fake_walls = 0;
    Position* arr_fake_walls = nullptr;
    string sherlock_moving_rule = "";
    Position sherlock_init_pos = Position::npos;
    int sherlock_init_hp = 0;
    int sherlock_init_exp = 0;
    string watson_moving_rule = "";
    Position watson_init_pos = Position::npos;
    int watson_init_hp = 0;
    int watson_init_exp = 0;
    Position criminal_init_pos = Position::npos;
    int num_steps = 0;

public:
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag,...
class Robot : public MovingObject
{
    friend class TestStudyInPink;

protected:
    RobotType robot_type;
    BaseItem* item;
    Criminal* criminal;
    Position original_pos;

public:
    Robot(int index, const Position &pos, Map* map, Criminal* criminal, const string &name = "");
    MovingObjectType getObjectType() const;
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    virtual string str() const = 0;
    virtual RobotType getRobotType();
    
    static Robot* createRobot(int index, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson);
    virtual int getDistance() const;
    BaseItem* getItem();
    Position CriminalPos();
};

class RobotC : public Robot
{
    friend class TestStudyInPink;

private:
public:
    RobotC(int index, const Position &init_pos, Map* map, Criminal* criminal);
    ~RobotC();
    Position getNextPosition();
    void move();
    int getDistance(Sherlock* sherlock);
    int getDistance(Watson* watson);
    string str() const;
};

class RobotS : public Robot
{
    friend class TestStudyInPink;

private:
    Sherlock* sherlock;

public:
    RobotS(int index, const Position &init_pos, Map* map, Criminal* criminal, Sherlock* sherlock);
    ~RobotS();
    Position getNextPosition();
    void move();
    int getDistance() const;
    string str() const;
};

class RobotW : public Robot
{
    friend class TestStudyInPink;

private:
    Watson* watson;

public:
    RobotW(int index, const Position &init_pos, Map* map, Criminal* criminal, Watson* watson);
    ~RobotW();
    Position getNextPosition();
    void move();
    int getDistance() const;
    string str() const;
};

class RobotSW : public Robot
{
    friend class TestStudyInPink;

private:
    Sherlock* sherlock;
    Watson* watson;

public:
    RobotSW(int index, const Position &init_pos, Map* map, Criminal* criminal, Sherlock* sherlock, Watson* watson);
    ~RobotSW();
    Position getNextPosition();
    string str() const;
    int getDistance() const;
    void move();
};

class BaseItem
{
    friend class TestStudyInPink;

public:
    virtual bool canUse(Character* obj, Robot* robot) = 0;
    virtual void use(Character* obj, Robot* robot) = 0;
    BaseItem();
    virtual ItemType getItemType() const = 0;
    virtual string str() const = 0;
    // virtual ~BaseItem() = default;
};

class MagicBook : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    ItemType getItemType() const;
    string str() const;
};

class EnergyDrink : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    ItemType getItemType() const;
    string str() const;
};

class FirstAid : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    ItemType getItemType() const;
    string str() const;
};

class ExcemptionCard : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    ItemType getItemType() const;
    string str() const;
};

class PassingCard : public BaseItem
{
    friend class TestStudyInPink;

private:
    string challenge;

public:
    PassingCard(int i, int j);
    PassingCard(string in_challenge);
    bool canUse(Character* obj, Robot* robot);
    void use(Character* obj, Robot* robot);
    ItemType getItemType() const;
    string str() const;
};

class BaseBag
{
    friend class TestStudyInPink;

protected:
    
    class Node
    {
    public:
        BaseItem* item;
        Node* next;

    public:
        Node(BaseItem* item, Node* next = nullptr) : item(item), next(next) {}
    };

protected:
    Character* obj;
    
    int size;
    int capacity;
    Node* head;

public:
    virtual bool insert(BaseItem* item);
    virtual BaseItem* get();
    virtual BaseItem* get(ItemType itemType);
    virtual string str() const;

    BaseBag(int capacity);
    virtual ~BaseBag();
    bool checkItem(ItemType itemType);
};

class SherlockBag : public BaseBag
{
    friend class TestStudyInPink;

private:
    Sherlock* sherlock;

public:
    SherlockBag(Sherlock* character);
    BaseItem* get();
};

class WatsonBag : public BaseBag
{
    friend class TestStudyInPink;

private:
    Watson* watson;

public:
    WatsonBag(Watson* character);
    BaseItem* get();
};

class StudyPinkProgram
{
    friend class TestStudyInPink;

private:
    Configuration* config;
    Sherlock* sherlock;
    Watson* watson;
    Criminal* criminal;
    Map* map;
    ArrayMovingObject* arr_mv_objs;
    bool stopChecker;

public:
    StudyPinkProgram(const string &config_file_path);
    bool isStop() const;
    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Watson caught the criminal" << endl;
        }
        else
        {
             cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
              << "--"
              << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }
    void run(bool verbose);
    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */