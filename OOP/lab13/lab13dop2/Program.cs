
class Program
{
    static void Main(string[] args)
    {
        Dictionary<string, List<string>> phoneBook = new Dictionary<string, List<string>>();
        Dictionary<string, string> reverseBook = new Dictionary<string, string>();

        AddContact(phoneBook, reverseBook, "Иванов", "+375291112233");
        AddContact(phoneBook, reverseBook, "Петров", "+375292223344");
        AddContact(phoneBook, reverseBook, "Сидорова", "+375293334455");
        AddContact(phoneBook, reverseBook, "Иванов", "+375294445566");

        AddPhone(phoneBook, reverseBook, "Петров", "+375295556677");
        AddPhone(phoneBook, reverseBook, "Козлов", "+375296667788");
        AddPhone(phoneBook, reverseBook, "Сидорова", "+375291112233");

        FindBySurname(phoneBook, "Иванов");
        FindBySurname(phoneBook, "Козлов");

        FindByPhone(reverseBook, "+375292223344");
        FindByPhone(reverseBook, "+375299999999");

        RemovePhone(phoneBook, reverseBook, "Иванов", "+375294445566");
        RemovePhone(phoneBook, reverseBook, "Иванов", "+375299999999");
        RemoveContact(phoneBook, reverseBook, "Петров");
        RemoveContact(phoneBook, reverseBook, "Козлов");
        RemovePhone(phoneBook, reverseBook, "Сидорова", "+375293334455");
    }

    static void AddContact(Dictionary<string, List<string>> phoneBook, Dictionary<string, string> reverseBook, string surname, string phone)
    {
        if (phoneBook.ContainsKey(surname))
        {
            AddPhone(phoneBook, reverseBook, surname, phone);
            return;
        }

        if (reverseBook.ContainsKey(phone))
        {
            Console.WriteLine($"Телефон {phone} уже записан на {reverseBook[phone]}");
            return;
        }

        List<string> phones = new List<string>();
        phones.Add(phone);
        phoneBook.Add(surname, phones);
        reverseBook.Add(phone, surname);

        Console.WriteLine($"Добавлен контакт: {surname} — {phone}");
    }

    static void AddPhone(Dictionary<string, List<string>> phoneBook, Dictionary<string, string> reverseBook, string surname, string phone)
    {
        if (!phoneBook.ContainsKey(surname))
        {
            Console.WriteLine($"Контакт {surname} не найден");
            return;
        }

        if (reverseBook.ContainsKey(phone))
        {
            Console.WriteLine($"Телефон {phone} уже записан на {reverseBook[phone]}");
            return;
        }

        phoneBook[surname].Add(phone);
        reverseBook.Add(phone, surname);

        Console.WriteLine($"Контакту {surname} добавлен телефон: {phone}");
    }

    static void RemoveContact(Dictionary<string, List<string>> phoneBook, Dictionary<string, string> reverseBook, string surname)
    {
        if (!phoneBook.TryGetValue(surname, out List<string>? phones))
        {
            Console.WriteLine($"Контакт {surname} не найден");
            return;
        }

        foreach (string phone in phones)
        {
            reverseBook.Remove(phone);
        }

        phoneBook.Remove(surname);

        Console.WriteLine($"Контакт {surname} удален");
    }

    static void RemovePhone(Dictionary<string, List<string>> phoneBook, Dictionary<string, string> reverseBook, string surname, string phone)
    {
        if (!phoneBook.TryGetValue(surname, out List<string>? phones))
        {
            Console.WriteLine($"Контакт {surname} не найден");
            return;
        }

        if (!phones.Remove(phone))
        {
            Console.WriteLine($"У контакта {surname} нет телефона {phone}");
            return;
        }

        reverseBook.Remove(phone);

        Console.WriteLine($"У контакта {surname} удален телефон {phone}");

        if (phones.Count == 0)
        {
            phoneBook.Remove(surname);
            Console.WriteLine($"Контакт {surname} удален");
        }
    }

    static void FindBySurname(Dictionary<string, List<string>> phoneBook, string surname)
    {
        if (!phoneBook.TryGetValue(surname, out List<string>? phones))
        {
            Console.WriteLine($"Контакт {surname} не найден");
            return;
        }

        Console.Write($"Телефоны контакта {surname}: ");
        foreach (string phone in phones)
        {
            Console.Write($"{phone} ");
        }

        Console.WriteLine();
    }

    static void FindByPhone(Dictionary<string, string> reverseBook, string phone)
    {
        if (reverseBook.TryGetValue(phone, out string? surname))
        {
            Console.WriteLine($"Телефон {phone} принадлежит {surname}");
        }
        else
        {
            Console.WriteLine($"Телефон {phone} не найден в книге");
        }
    }
}
