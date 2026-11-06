delegate int Operation(int a, int b);

class Calculator
{
    public int Add(int a, int b)
    {
        return a + b;
    }

    public int Subtract(int a, int b)
    {
        return a - b;
    }

    public int Multiply(int a, int b)
    {
        return a * b;
    }

    public int Divide(int a, int b)
    {
        if (b == 0)
        {
            throw new DivideByZeroException("Деление на 0 недопустимо");
        }
        return a / b;
    }
}

class TickEventArgs : EventArgs
{
    public int TickNumber { get; set; }
    public DateTime Time { get; set; }

    public TickEventArgs(int number)
    {
        TickNumber = number;
        Time = DateTime.Now;
    }
}

class Timer
{
    public event EventHandler<TickEventArgs> Tick;

    public void Start(int ticksCount)
    {
        for (int i = 1; i <= ticksCount; i++)
        {
            Thread.Sleep(1000);
            Tick?.Invoke(this, new TickEventArgs(i));
        }
    }
}


class Program
{
    static void ShowTime(object sender, TickEventArgs e)
    {
        Console.WriteLine($"Время: {e.Time:HH:mm:ss}  (тик {e.TickNumber})");
    }
    
    static void Beep(object sender, TickEventArgs e)
    {
        try { Console.Beep(800, 100); } catch { }
        Console.WriteLine($"Звук {e.TickNumber})");
    }

    static void Log(object sender, TickEventArgs e)
    {
        Console.WriteLine($"Тик {e.TickNumber} записан в {e.Time:HH:mm:ss}");
    }
    
    static void Main(string[] args)
    {
        int a = 10;
        int b = 25;
        Calculator calculator = new Calculator();
        Operation[] operations =
        {
            calculator.Add,
            calculator.Subtract,
            calculator.Multiply,
            calculator.Divide
        };
        for (int i = 0; i < operations.Length; i++)
        {
            int result = operations[i](a, b);
            Console.WriteLine($"Операция {i + 1}: {a}, {b} = {result}");
        }
        
        Timer timer = new Timer();
        timer.Tick += ShowTime;
        timer.Tick += Beep;
        timer.Tick += Log;

        Console.WriteLine("Подписано 3 обработчика.\n");
        
        timer.Start(5);

        Console.WriteLine("\nТаймер остановлен.");
    }
}