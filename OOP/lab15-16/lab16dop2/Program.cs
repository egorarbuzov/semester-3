using System.Reflection;

enum MaskPattern
{
    FirstChars,
    Email,
    Phone,
    Card
}

[AttributeUsage(AttributeTargets.Property)]
class MaskAttribute : Attribute
{
    public int VisibleCount { get; set; } = 3;
    public char MaskChar { get; set; } = '*';
    public MaskPattern Pattern { get; set; } = MaskPattern.FirstChars;

    public string Apply(string value)
    {
        if (string.IsNullOrEmpty(value))
        {
            return value;
        }

        if (Pattern == MaskPattern.Email)
        {
            return MaskEmail(value);
        }

        if (Pattern == MaskPattern.Phone)
        {
            return MaskNumbers(value, 2);
        }

        if (Pattern == MaskPattern.Card)
        {
            return MaskNumbers(value, 4);
        }

        if (value.Length <= VisibleCount)
        {
            return new string(MaskChar, value.Length);
        }

        return value.Substring(0, VisibleCount) + new string(MaskChar, value.Length - VisibleCount);
    }

    private string MaskEmail(string value)
    {
        int atIndex = value.IndexOf('@');
        if (atIndex <= 1)
        {
            return new string(MaskChar, value.Length);
        }

        string localPart = value.Substring(0, atIndex);
        string domain = value.Substring(atIndex);
        return localPart[0] + new string(MaskChar, localPart.Length - 1) + domain;
    }

    private string MaskNumbers(string value, int visibleDigits)
    {
        char[] chars = value.ToCharArray();
        int digits = 0;
        for (int i = chars.Length - 1; i >= 0; i--)
        {
            if (char.IsDigit(chars[i]))
            {
                digits++;
                if (digits > visibleDigits)
                {
                    chars[i] = MaskChar;
                }
            }
        }

        return new string(chars);
    }
}

class User
{
    [Mask(VisibleCount = 2, MaskChar = '#')]
    public string Login { get; set; } = string.Empty;

    [Mask(Pattern = MaskPattern.Email)]
    public string Email { get; set; } = string.Empty;

    [Mask(Pattern = MaskPattern.Phone)]
    public string Phone { get; set; } = string.Empty;

    [Mask(Pattern = MaskPattern.Card)]
    public string CardNumber { get; set; } = string.Empty;

    public string City { get; set; } = string.Empty;
}

class DataMasker
{
    public static object MaskObject(object source)
    {
        Type type = source.GetType();
        object copy = Activator.CreateInstance(type)!;

        foreach (PropertyInfo property in type.GetProperties())
        {
            if (!property.CanWrite)
            {
                continue;
            }

            object? value = property.GetValue(source);
            MaskAttribute? attribute = property.GetCustomAttribute<MaskAttribute>();
            if (attribute == null)
            {
                property.SetValue(copy, value);
                continue;
            }

            string text = value?.ToString() ?? string.Empty;
            property.SetValue(copy, attribute.Apply(text));
        }

        return copy;
    }
}

class Program
{
    static void Main(string[] args)
    {
        User user = new User
        {
            Login = "ivan_petrov",
            Email = "ivan.petrov@mail.ru",
            Phone = "+375 (29) 123-45-67",
            CardNumber = "1234 5678 9012 3456",
            City = "Минск"
        };

        PrintUser("Исходный объект", user);

        User maskedUser = (User)DataMasker.MaskObject(user);

        Console.WriteLine();
        PrintUser("Замаскированная копия", maskedUser);

        Console.WriteLine("\nИсходный объект не изменился:");
        Console.WriteLine($"  Логин: {user.Login}");
        Console.WriteLine($"  Email: {user.Email}");
    }

    static void PrintUser(string title, User user)
    {
        Console.WriteLine(title);
        Console.WriteLine($"  Логин: {user.Login}");
        Console.WriteLine($"  Email: {user.Email}");
        Console.WriteLine($"  Телефон: {user.Phone}");
        Console.WriteLine($"  Карта: {user.CardNumber}");
        Console.WriteLine($"  Город: {user.City}");
    }
}
