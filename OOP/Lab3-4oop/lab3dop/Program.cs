using System;

class Logger
{
    static public int InstanceCount;

    public Logger()
    {
        InstanceCount++;
        Console.WriteLine("Объект создан");
    }
    public static int GetInstanceCount() {
        return InstanceCount;
    }
}

class Program
{
    static void Main(string[] args)
    {
        Logger log1 = new Logger();
        Logger log2 = new Logger();
        Console.WriteLine(Logger.GetInstanceCount()); 
        Logger log3 = new Logger();
        Logger log4 = new Logger();
        Logger log5 = new Logger();
        Console.WriteLine(Logger.GetInstanceCount());
    }
}