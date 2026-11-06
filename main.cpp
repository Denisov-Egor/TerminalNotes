#include <iostream>
#include <fstream>

#include <string>
#include <limits>

void Menu()
{
  std::cout << R"(
  ==========================================
                TERMINAL NOTES
  ==========================================

  [ ЗАМЕТКИ ]

  1. Создать заметку
  2. Показать все заметки
  3. Открыть заметку
  4. Редактировать заметку
  5. Удалить заметку
  6. Найти заметку

  [ ФАЙЛЫ ]

  7. Загрузить заметки из файла
  8. Сохранить заметки в файл

  [ ПРОГРАММА ]

  0. Выход

  ==========================================

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

  void clear()
  {
    noteCount = 0;
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
  std::string line;

  std::cout << "Введите заголовок: ";
  if (!std::getline(std::cin, title))
  {
    return;
  }

  std::cout << "Введите содержимое (для завершения введите точку .):\n";

  bool firstLine = true;

  while (std::getline(std::cin, line))
  {
    if (line == ".")
    {
      break;
    }

    if (!firstLine)
    {
      content += '\n';
    }

    content += line;
    firstLine = false;
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

bool SaveNotes(const NoteManager& manager)
{
  std::ofstream file("notes.txt");

  if (!file.is_open())
  {
    return false;
  }
  
  int noteCount = manager.getNoteCount();

  for (int index = 0; index < noteCount; ++index)
  {
    const Note* note = manager.getNote(index);

    if (note == nullptr)
    {
      return false;
    }
    
    file << "[NOTE]\n";

    if (note->getTitle().empty())
    {
      file << "Заголовок пуст\n";
    }else
    {
      file << note->getTitle() << '\n';
    }
    
    if (note->getContent().empty())
    {
      file << "Содержание пуст\n";
    }else
    {
      file << note->getContent() << '\n';
    }
    
    file << "[END]\n\n";
  }

  file.close();
  
  if (!file)
  {
    return false;
  }

  return true;
}

bool LoadNotes(NoteManager& manager)
{
  std::string marker;
  std::string title;
  std::string content;

  NoteManager tempManager;

  std::ifstream file("notes.txt");

  if (!file.is_open())
  {
    return false;
  }
  
  while (std::getline(file, marker))
  {
    if (marker.empty())
    {
      continue;
    }
    
    if (marker != "[NOTE]")
    {
      return false;
    }
    
    if (!std::getline(file, title))
    {
      return false;
    }
    
    if (title == "Заголовок пуст")
    {
      title.clear();
    }

    content.clear();

    bool firstLine = true;

    while (std::getline(file, marker) && marker != "[END]")
    {
      if (!firstLine)
      {
        content += '\n';
      }

      content += marker;
      firstLine = false;
    }

    if (marker != "[END]")
    {
      return false;
    }

    if (content == "Содержание пуст")
    {
      content.clear();
    }

    Note note;

    note.setTitle(title);
    note.setContent(content);

    if (!tempManager.addNote(note))
    {
      return false;
    }
  }
  
  if (file.bad())
  {
    return false;
  }

  manager.clear();

  int noteCount = tempManager.getNoteCount();

  for (int index = 0; index < noteCount; ++index)
  {
    const Note* note = tempManager.getNote(index);

    if (note == nullptr)
    {
      return false;
    }

    if (!manager.addNote(*note))
    {
      return false;
    }
  }

  return true;
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

      choice = -1;

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

    case 4:

      break;

    case 5:

      break;

    case 6:

      break;

    case 7:
      if (LoadNotes(manager))
      {
        std::cout << "Заметки успешно загружены.\n";
      }else
      {
        std::cout << "Ошибка: не удалось загрузить заметки.\n";
      }      
      break;

    case 8:
      if (SaveNotes(manager))
      {
        std::cout << "Заметки успешно сохранены.\n";
      }else
      {
        std::cout << "Ошибка: не удалось сохранить заметки.\n";
      }
      break;

    case 0:
      std::cout << "Выход.\n";
      break;

    default:
      std::cout << "Неверный пункт меню.\n";
      break;
    }

    if (std::cin.eof())
    {
      std::cout << "\nЗавершение программы.\n";
      break;
    }
  }
}