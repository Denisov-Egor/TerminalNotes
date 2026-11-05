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
  void setTitle(const std::string& newTitle)
  {
    title = newTitle;
  }

  void setContent(const std::string& newContent)
  {
    content = newContent;
  }

  const std::string& getTitle() const
  {
    return title;
  }

  const std::string& getContent() const
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
    return noteCount == 0;
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
    ++noteCount;

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
  if (!std::getline(std::cin, title))
  {
    return;
  }

  std::cout << "Введите содержимое: ";
  if (!std::getline(std::cin, content))
  {
    return;
  }

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

void ShowNotes(const NoteManager& manager)
{
  if (manager.isEmpty())
  {
    std::cout << "Ошибка: заметки пока не созданы.\n";
    return;
  }
  
  int noteCount = manager.getNoteCount();

  std::cout << R"(
  ====================================
                NOTES
  ====================================
  )";

  for (int index = 0; index < noteCount; ++index)
  {
    const Note* note = manager.getNote(index);

    if (note == nullptr)
    {
      continue;
    }

    std::cout << "[" << index << "] " << note->getTitle() << '\n';
  }
  
  std::cout << "====================================\n";
}

void OpenNote(const NoteManager& manager)
{
  if (manager.isEmpty())
  {
    std::cout << "Ошибка: заметки пока не созданы.\n";
    return;
  }

  int index = -1;

  std::cout << "Введите индекс заметки: ";
  
  if (!(std::cin >> index))
  {
    if (std::cin.eof())
    {
      return;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Ошибка: необходимо ввести число.\n";
    return;
  }

  const Note* note = manager.getNote(index);

  if (note == nullptr)
  {
    std::cout << "Ошибка: заметка с таким индексом не найдена.\n";
    return;
  }

  std::cout << R"(
  ====================================
               OPEN NOTE
  ====================================
  )";

  std::cout << "Заголовок: " << note->getTitle() << "\n\n";
  std::cout << "Содержимое:\n";
  std::cout << note->getContent() << '\n';

  std::cout << "====================================\n";

}

int main()
{
  int choice = -1;
  NoteManager manager;

  while (choice != 0)
  {
    Menu();

    if (!(std::cin >> choice))
    {
      if (std::cin.eof())
      {
        std::cout << "\nЗавершение программы.\n";
        break;
      }

      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

      std::cout << "Ошибка: необходимо ввести число.\n";
      continue;
    }

    switch (choice)
    {
    case 1:
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      CreateNote(manager);
      break;
    
    case 2:
      ShowNotes(manager);
      break;
    
    case 3:
      OpenNote(manager);
      break;
    
    case 0:
      std::cout << "Выход.";
      break;
    
    default:
      std::cout << "Неверный пункт меню: ";
      break;
    }
     
    if (std::cin.eof())
    {
      break;
    }
  }
}