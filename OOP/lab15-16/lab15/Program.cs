using System.Reflection;

class Book
{
    private string title;
    private string author;
    private int year;

    public string Title
    {
        get { return title; }
        set { title = value; }
    }

    public string Author
    {
        get { return author; }
        set { author = value; }
    }

    public int Year
    {
        get { return year; }
        set { year = value; }
    }

    public Book()
    {
        title = "Неизвестно";
        author = "Неизвестно";
        year = 0;
    }

    public Book(string title, string author, int year)
    {
        this.title = title;
        this.author = author;
        this.year = year;
    }

    public void ShowInfo()
    {
        Console.WriteLine($"{title} — {author}, {year} год");
    }

    public int GetAge(int currentYear)
    {
        return currentYear - year;
    }
}

class Program
{
    static void Main(string[] args)
    {
        Type bookType = typeof(Book);

        Console.WriteLine("Информация о классе Book");
        Console.WriteLine($"Имя типа: {bookType.Name}");
        Console.WriteLine($"Полное имя: {bookType.FullName}");
        Console.WriteLine($"Пространство имен: {bookType.Namespace ?? "нет"}");
        Console.WriteLine($"Базовый класс: {bookType.BaseType?.Name ?? "нет"}");
        Console.WriteLine($"Это класс: {bookType.IsClass}");

        Console.WriteLine("\nПоля класса Book");
        FieldInfo[] fields = bookType.GetFields(BindingFlags.Instance | BindingFlags.NonPublic);
        foreach (FieldInfo field in fields)
        {
            Console.WriteLine($"  {field.FieldType.Name} {field.Name} (закрытое: {field.IsPrivate})");
        }

        Console.WriteLine("\nСвойства класса Book");
        PropertyInfo[] properties = bookType.GetProperties();
        foreach (PropertyInfo property in properties)
        {
            Console.WriteLine($"  {property.PropertyType.Name} {property.Name} (читать: {property.CanRead}, записывать: {property.CanWrite})");
        }

        Console.WriteLine("\nМетоды класса Book");
        MethodInfo[] methods = bookType.GetMethods();
        foreach (MethodInfo method in methods)
        {
            if (method.DeclaringType != bookType || method.IsSpecialName)
            {
                continue;
            }

            Console.WriteLine($"{method.ReturnType.Name} {method.Name}");
            ParameterInfo[] parameters = method.GetParameters();
            foreach (ParameterInfo parameter in parameters)
            {
                Console.WriteLine($"параметр: {parameter.ParameterType.Name} {parameter.Name}");
            }
        }

        Console.WriteLine("\nИнформация о текущей сборке");
        Assembly assembly = Assembly.GetExecutingAssembly();
        Console.WriteLine($"Имя сборки: {assembly.GetName().Name}");
        Console.WriteLine($"Версия сборки: {assembly.GetName().Version}");
        Console.WriteLine($"Расположение: {assembly.Location}");

        Console.WriteLine("\nТипы в текущей сборке");
        Type[] types = assembly.GetTypes();
        Console.WriteLine($"Всего типов: {types.Length}");
        foreach (Type type in types)
        {
            Console.WriteLine($"{type.Name}");
        }
    }
}
