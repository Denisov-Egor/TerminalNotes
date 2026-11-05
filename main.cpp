#include <iostream>
#include <string>
#include <limits>

void Menu()
{
  std::cout << R"(
  ====================================
           TERMINAL NOTES
  ====================================

    1. Создать заметку
    2. Показать все заметки
    3. Открыть заметку

    0. Выход

  ====================================

  Выберите действие:
  )";
}

class Note
{
private:
  std::string title = "Новый заголовок";
  std::string content;
public:
  bool setTitle(const std::string& newTitle)
  {  
    title = newTitle;
    return true;
  }

  bool setContent(const std::string& newContent)
  {
    content = newContent;
    return true;
  }

  std::string getTitle() const
  {
    return title;
  }

  std::string getContent() const
  {
    return content;
  }
};

class NoteManager
{
private:
  Note notes[100];
  int noteCount = 0;
public:
  bool isEmpty() const
  {
    return noteCount <= 0;
  }

  bool isFull() const
  {
    return noteCount >= 100;
  }

  int getNoteCount() const
  {
    return noteCount; 
  }

  bool addNote(const Note& note)
  {
    if (isFull())
    {
      return false;
    }
    
    notes[noteCount] = note;
    noteCount++;

    return true;
  }

  const Note* getNote(int index) const
  {
    if (index < 0 || index >= noteCount)
    {
      return nullptr;
    }
    
    return &notes[index];
  }
};
  
void CreateNote(NoteManager& manager)
{
   if (manager.isFull())
  {
    std::cout << "Ошибка: хранилище заметок заполнено.\n";
    return;
  }

  Note note;

  std::string title;
  std::string content;

  std::cout << "Введите заголовок: ";
  std::getline(std::cin, title);

  std::cout << "Введите содержимое: ";
  std::getline(std::cin, content);

  note.setTitle(title);
  note.setContent(content);

  if (manager.addNote(note))
  {
    std::cout << "Заметка успешно создана.\n";
  }else
  {
    std::cout << "Ошибка: не удалось добавить заметку.\n";
  }
}
 
int main()
{
  int choice = -1;
  NoteManager manager;

  while (choice != 0)
  {
    Menu();
    std::cin >> choice;

    switch (choice)
    {
    case 1:
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      CreateNote(manager);
      break;
    
    case 2:
      
      break;
    
    case 3:
      
      break;
    
    case 0:
      std::cout << "Выход.";
      break;
    
    default:
      std::cout << "Неверный пункт меню: ";
      break;
    }
  }
  

}