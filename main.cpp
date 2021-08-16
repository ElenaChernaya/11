#include <iostream>
#include <string>
#include <vector>

class Branch {
    Branch* parent = nullptr; //дерево
    std::vector<Branch*> children; //вектор деревьев с ветками

    std::string elfName = "None"; //имя эльфа

public:

    //конструктор дерева
    Branch(Branch* inParent) {
        parent = inParent; //объект дерева (присваивается кол-во деревьев)
    }

    //метод добавления веток (один метод для больших и средних веток)
    Branch* addChild() {
        Branch* child = new Branch(this); //выделяем память для ветки
        return child; //возвращает ветку
    }

    //метод получения размера вектора веток, возвращающий размер самого ветора для рандомного присваивания
    int getChildrenCount() {
        return children.size();
    }

    //метод получения ветки по индексу
    Branch* getChildAt(int index) {
        return children[index];
    }

    //метод проверки возвращающий true
    bool canBeOccupied() {
        return parent != nullptr; //дерево не равно нулевому указателю
    }

    //метод доступа к имени эльфа
    std::string getElfName() {
        return elfName; //возвращает имя эльфа
    }

    //если такое имя эльфа уже есть на ветке - возвращаемся в начало метода
    //если эльф, введенный с клавы не равен None, проверяем: если на ветке уже есть эльф,
    // если ветка имеет нулевой указатель - она не может быть заселена
    //присваиваем эльфу имя с клавы
    void occupy(std::string inElfName) {
        if (elfName == inElfName) return;
        if (inElfName != "None") {
            if (elfName != "None") {
                std::cerr << "House already occupied!" << std::endl;
                return;
            }
            if (!canBeOccupied()) {
                std::cerr << "This branch can't be occupied!" << std::endl;
                return;
            }
            elfName = inElfName;
        }
    }

    // метод доступа к верхней ветке
    Branch* getTopBranch() {
        if (parent == nullptr) return nullptr;
        if (parent->parent == nullptr) return parent;
        return parent->getTopBranch(); //возвращает указатель на метод
    }

    //метод получения больших веток
    Branch* getBigBranch() {
        if (parent == nullptr) return nullptr;
        return parent->parent == nullptr ? this : parent;
    }

    //метод получения ...
    int countElves() {
        int count = elfName != "None";
        for (int i = 0; i < children.size(); ++i) {
            count += children[i]->countElves();
        }
        return count;
    }

    //метод получения кол-ва соседей эльфа
    int countNeighbors() {
        Branch* bigBranch = getBigBranch();
        return bigBranch == nullptr ? 0 : bigBranch->countElves() - (elfName != "None"); //кол-во эльфов на большой ветке минус ...
    }

    //метод для ввода имен эльфов и заселение их на ветки
    void occupyChildren() {
        if (canBeOccupied()) { // если дерево не пустое ...
            std::string inElfName;
            std::cout << "Elf name:";
            std::cin >> inElfName;
            occupy(inElfName); //...вызывается метод проверки за занятость и выводится сообщение, что дом уже занят
        }

        for (int i = 0; i < children.size(); ++i) {
            Branch* child = children[i];
            child->occupyChildren();
        }
    }

    //метод поиска эльфа по имени введенном пользователем
    Branch* findElfBranch(const std::string& inElfName) {
        if (elfName == inElfName) {
            return this;
        }
        for (int i = 0; i < children.size(); ++i) {

            if (Branch* child = children[i]->findElfBranch(inElfName); child != nullptr) {
                return child;
            }
        }
        return nullptr;
    }
};

int main() {
    std::vector<Branch*> trees; //вектор деверьев с указателем на класс с ветками и эльфами
    for (int i = 0; i < 1; ++i) { // 1 для простоты
        Branch* tree = new Branch(nullptr);
        trees.push_back(tree);
        int largeCount = (std::rand() % 3) + 3;
        for (int j = 0; j < largeCount; ++j) {
            Branch* large = tree->addChild(); //пока не конец цикла, вызывается метод добавления массива веток в вектор дерева
            int smallCount = (std::rand() % 2) + 2;
            for (int k = 0; k < smallCount; ++k) {
                large->addChild(); //пока не конец цикла, вызывается метод добавления массива веток в вектор дерева
            }
        }
    }

    for (int i = 0; i < trees.size(); ++i) {
        trees[i]->occupyChildren(); // пока не конец вектора с числом деревьев, вызывается метод ввода имен эльфов и проверка на то, что ветка еще не занята
    }

    std::string elfName;
    std::cout << "Search for elf:";
    std::cin >> elfName;

    //поиск по вектору с деревьями имени эльфа, введенного пользователем и подсчет кол-ва его соседей
    for (int i = 0; i < trees.size(); ++i) {
        if (Branch* branch = trees[i]->findElfBranch(elfName);branch != nullptr) {
            int count = branch->countNeighbors();
            std::cout << "Neighbors: " << count << std::endl;
        }
    }
}
