using System.Reflection;

interface IPrintable
{
    void Print();
}

class Book : IPrintable
{
    private string title = string.Empty;
    protected string author = string.Empty;
    public int year;
    internal decimal price;

    public string Title
    {
        get { return title; }
        set { title = value; }
    }

    public string Author
    {
        get { return author; }
    }

    public decimal Price
    {
        get { return price; }
        set { price = value; }
    }

    public Book()
    {
        title = "Неизвестно";
        author = "Неизвестно";
    }

    public Book(string title, string author, int year, decimal price)
    {
        this.title = title;
        this.author = author;
        this.year = year;
        this.price = price;
    }

    public void Print()
    {
        Console.WriteLine($"{title} — {author}, {year}, {price:F2}");
    }

    public int GetAge(int currentYear)
    {
        return currentYear - year;
    }

    public static Book CreateDefault()
    {
        return new Book("Без названия", "Без автора", 2000, 0m);
    }

    private void Log(string message)
    {
        Console.WriteLine($"  {message}");
    }
}

class Program
{
    static void Main(string[] args)
    {
        ExploreType(typeof(Book));
    }

    static void ExploreType(Type type)
    {
        Console.WriteLine($"Тип: {type.Name}");
        Console.WriteLine($"Полное имя: {type.FullName ?? "нет"}");
        Console.WriteLine($"Пространство имен: {type.Namespace ?? "нет"}");
        Console.WriteLine($"Базовый класс: {type.BaseType?.Name ?? "нет"}");

        Type[] interfaces = type.GetInterfaces();
        Console.Write("Интерфейсы: ");
        if (interfaces.Length == 0)
        {
            Console.WriteLine("нет");
        }
        else
        {
            for (int i = 0; i < interfaces.Length; i++)
            {
                if (i > 0)
                {
                    Console.Write(", ");
                }

                Console.Write(interfaces[i].Name);
            }

            Console.WriteLine();
        }

        BindingFlags flags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance |
                             BindingFlags.Static | BindingFlags.DeclaredOnly;

        Console.WriteLine("\nПоля:");
        FieldInfo[] fields = type.GetFields(flags);
        foreach (FieldInfo field in fields)
        {
            Console.WriteLine($"  {GetAccess(field.IsPublic, field.IsPrivate, field.IsFamily, field.IsAssembly)} {field.FieldType.Name} {field.Name}");
        }

        Console.WriteLine("\nСвойства:");
        PropertyInfo[] properties = type.GetProperties(flags);
        foreach (PropertyInfo property in properties)
        {
            Console.WriteLine($"  {property.PropertyType.Name} {property.Name}");
            Console.WriteLine($"    читать: {property.CanRead}, записывать: {property.CanWrite}");
        }

        Console.WriteLine("\nМетоды:");
        MethodInfo[] methods = type.GetMethods(flags);
        foreach (MethodInfo method in methods)
        {
            if (method.IsSpecialName)
            {
                continue;
            }

            string modifier = method.IsStatic ? "static " : "";
            Console.WriteLine($"  {GetAccess(method.IsPublic, method.IsPrivate, method.IsFamily, method.IsAssembly)} {modifier}{method.ReturnType.Name} {method.Name}({GetParameters(method.GetParameters())})");
        }

        Console.WriteLine("\nКонструкторы:");
        ConstructorInfo[] constructors = type.GetConstructors(flags);
        foreach (ConstructorInfo constructor in constructors)
        {
            Console.WriteLine($"  {GetAccess(constructor.IsPublic, constructor.IsPrivate, constructor.IsFamily, constructor.IsAssembly)} {type.Name}({GetParameters(constructor.GetParameters())})");
        }
    }

    static string GetAccess(bool isPublic, bool isPrivate, bool isFamily, bool isAssembly)
    {
        if (isPublic)
        {
            return "public";
        }

        if (isPrivate)
        {
            return "private";
        }

        if (isFamily)
        {
            return "protected";
        }

        if (isAssembly)
        {
            return "internal";
        }

        return "другое";
    }

    static string GetParameters(ParameterInfo[] parameters)
    {
        if (parameters.Length == 0)
        {
            return "нет параметров";
        }

        string result = "";
        for (int i = 0; i < parameters.Length; i++)
        {
            if (i > 0)
            {
                result += ", ";
            }

            result += $"{parameters[i].ParameterType.Name} {parameters[i].Name}";
        }

        return result;
    }
}
