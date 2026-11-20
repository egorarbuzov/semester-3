using System.Reflection;

[AttributeUsage(AttributeTargets.Property)]
class TrackChangesAttribute : Attribute
{
}

class Employee
{
    public int Id { get; set; }

    [TrackChanges]
    public string Name { get; set; } = string.Empty;

    [TrackChanges]
    public string Position { get; set; } = string.Empty;

    [TrackChanges]
    public decimal Salary { get; set; }
}

class ChangeTracker
{
    private object target;
    private Dictionary<string, object?> savedValues = new Dictionary<string, object?>();

    public ChangeTracker(object target)
    {
        this.target = target;
        SaveValues();
    }

    public List<string> GetChanges()
    {
        List<string> changes = new List<string>();
        foreach (KeyValuePair<string, object?> pair in savedValues)
        {
            PropertyInfo? property = target.GetType().GetProperty(pair.Key);
            if (property == null)
            {
                continue;
            }

            object? currentValue = property.GetValue(target);
            if (!Equals(pair.Value, currentValue))
            {
                changes.Add($"{property.Name}: было \"{pair.Value}\", стало \"{currentValue}\"");
            }
        }

        return changes;
    }

    public void AcceptChanges()
    {
        savedValues.Clear();
        SaveValues();
    }

    private void SaveValues()
    {
        foreach (PropertyInfo property in target.GetType().GetProperties())
        {
            if (property.GetCustomAttribute<TrackChangesAttribute>() != null)
            {
                savedValues[property.Name] = property.GetValue(target);
            }
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        Employee employee = new Employee
        {
            Id = 1,
            Name = "Иван Петров",
            Position = "Программист",
            Salary = 65000m
        };

        ChangeTracker tracker = new ChangeTracker(employee);

        PrintEmployee("Сотрудник до изменений", employee);

        employee.Position = "Старший программист";
        employee.Salary = 78000m;
        employee.Id = 2;

        Console.WriteLine();
        PrintEmployee("Сотрудник после изменений", employee);

        Console.WriteLine();
        ShowChanges(tracker.GetChanges());

        Console.WriteLine("\nПринимаем изменения");
        tracker.AcceptChanges();
        ShowChanges(tracker.GetChanges());

        employee.Name = "Иван Сидоров";

        Console.WriteLine("\nМеняем имя сотрудника");
        ShowChanges(tracker.GetChanges());
    }

    static void PrintEmployee(string title, Employee employee)
    {
        Console.WriteLine(title);
        Console.WriteLine($"  Id: {employee.Id}");
        Console.WriteLine($"  Имя: {employee.Name}");
        Console.WriteLine($"  Должность: {employee.Position}");
        Console.WriteLine($"  Оклад: {employee.Salary:F2}");
    }

    static void ShowChanges(List<string> changes)
    {
        Console.WriteLine("Измененные свойства:");
        if (changes.Count == 0)
        {
            Console.WriteLine("  изменений нет");
            return;
        }

        foreach (string change in changes)
        {
            Console.WriteLine($"  {change}");
        }
    }
}
