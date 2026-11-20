[Serializable]
class Person
{
    public string Name = string.Empty;
    public int Age;
    public string Email = string.Empty;

    [NonSerialized]
    public string Password = string.Empty;
}

class Program
{
    static void Main(string[] args)
    {
        string fileName = "person.dat";

        Person person = new Person
        {
            Name = "Иван Петров",
            Age = 25,
            Email = "ivan.petrov@mail.ru",
            Password = "secret123"
        };

        SavePerson(person, fileName);
        Person loadedPerson = LoadPerson(fileName);

        Console.WriteLine($"Сохраненный объект: {person.Name}, {person.Age}, {person.Email}");
        Console.WriteLine($"Загруженный объект: {loadedPerson.Name}, {loadedPerson.Age}, {loadedPerson.Email}");
        Console.WriteLine($"Пароль после загрузки: \"{loadedPerson.Password}\" (поле с [NonSerialized] не сохранилось)");
    }

    static void SavePerson(Person person, string fileName)
    {
        using (FileStream stream = new FileStream(fileName, FileMode.Create))
        using (BinaryWriter writer = new BinaryWriter(stream))
        {
            writer.Write(person.Name);
            writer.Write(person.Age);
            writer.Write(person.Email);
        }
    }

    static Person LoadPerson(string fileName)
    {
        Person person = new Person();

        using (FileStream stream = new FileStream(fileName, FileMode.Open))
        using (BinaryReader reader = new BinaryReader(stream))
        {
            person.Name = reader.ReadString();
            person.Age = reader.ReadInt32();
            person.Email = reader.ReadString();
        }

        return person;
    }
}
