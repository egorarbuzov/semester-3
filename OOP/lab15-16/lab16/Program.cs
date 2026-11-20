using System.Diagnostics;
using System.Reflection;

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Method, AllowMultiple = true)]
class DocumentAttribute : Attribute
{
    public string Author { get; }
    public string Version { get; }
    public string Description { get; }

    public DocumentAttribute(string author, string version, string description)
    {
        Author = author;
        Version = version;
        Description = description;
    }
}

[AttributeUsage(AttributeTargets.Method)]
class ExecutionTimeAttribute : Attribute
{
    public void Measure(object target, MethodInfo method)
    {
        Stopwatch stopwatch = Stopwatch.StartNew();
        method.Invoke(target, null);
        stopwatch.Stop();
        Console.WriteLine($"Время выполнения метода {method.Name}: {stopwatch.ElapsedMilliseconds} мс");
    }
}

[Document("Иван Петров", "1.0", "Класс для обработки текста")]
class TextProcessor
{
    [Document("Иван Петров", "1.1", "Считает количество слов в тексте")]
    public int CountWords(string text)
    {
        string[] words = text.Split(' ', StringSplitOptions.RemoveEmptyEntries);
        return words.Length;
    }

    [Document("Мария Сидорова", "1.2", "Переводит текст в верхний регистр")]
    public string ToUpperCase(string text)
    {
        return text.ToUpper();
    }

    [Document("Иван Петров", "1.0", "Возвращает первое слово текста")]
    public string GetFirstWord(string text)
    {
        string[] words = text.Split(' ', StringSplitOptions.RemoveEmptyEntries);
        return words[0];
    }

    [ExecutionTime]
    public void CalculateSum()
    {
        long sum = 0;
        for (int i = 0; i < 10000000; i++)
        {
            sum += i;
        }

        Console.WriteLine($"Сумма: {sum}");
    }

    [ExecutionTime]
    public void SortArray()
    {
        Random random = new Random();
        int[] numbers = new int[100000];
        for (int i = 0; i < numbers.Length; i++)
        {
            numbers[i] = random.Next(1, 1000);
        }

        Array.Sort(numbers);
        Console.WriteLine($"Первый элемент после сортировки: {numbers[0]}");
    }
}

[Document("Мария Сидорова", "2.0", "Класс для формирования отчетов")]
class ReportBuilder
{
    [Document("Мария Сидорова", "2.1", "Формирует заголовок отчета")]
    public string BuildHeader(string title, DateTime date)
    {
        return $"{title} от {date:dd.MM.yyyy}";
    }

    [Document("Иван Петров", "2.0", "Формирует строку с итоговой суммой")]
    public string BuildTotal(decimal total)
    {
        return $"Итого: {total:F2}";
    }
}

class Program
{
    static void Main(string[] args)
    {
        TextProcessor processor = new TextProcessor();

        ShowDocumentation(typeof(TextProcessor));

        Console.WriteLine();
        ShowDocumentation(typeof(ReportBuilder));

        Console.WriteLine();
        Console.WriteLine("Замер времени выполнения методов");
        MeasureExecutionTime(processor);
    }

    static void ShowDocumentation(Type type)
    {
        Console.WriteLine("Документация из атрибутов");
        Console.WriteLine($"Класс: {type.Name}");

        DocumentAttribute? classAttribute = type.GetCustomAttribute<DocumentAttribute>();
        if (classAttribute == null)
        {
            Console.WriteLine("атрибут Document не найден");
        }
        else
        {
            Console.WriteLine($"Автор: {classAttribute.Author}");
            Console.WriteLine($"Версия: {classAttribute.Version}");
            Console.WriteLine($"Описание: {classAttribute.Description}");
        }

        Console.WriteLine("\nМетоды:");
        MethodInfo[] methods = type.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly);
        foreach (MethodInfo method in methods)
        {
            DocumentAttribute? methodAttribute = method.GetCustomAttribute<DocumentAttribute>();
            if (methodAttribute == null)
            {
                continue;
            }

            Console.WriteLine($"{method.Name}");
            Console.WriteLine($"Автор: {methodAttribute.Author}");
            Console.WriteLine($"Версия: {methodAttribute.Version}");
            Console.WriteLine($"Описание: {methodAttribute.Description}");
        }
    }

    static void MeasureExecutionTime(object target)
    {
        Type type = target.GetType();
        MethodInfo[] methods = type.GetMethods(BindingFlags.Public | BindingFlags.Instance | BindingFlags.DeclaredOnly);
        foreach (MethodInfo method in methods)
        {
            ExecutionTimeAttribute? attribute = method.GetCustomAttribute<ExecutionTimeAttribute>();
            if (attribute == null)
            {
                continue;
            }

            Console.WriteLine($"Вызов метода {method.Name}:");
            attribute.Measure(target, method);
        }
    }
}
