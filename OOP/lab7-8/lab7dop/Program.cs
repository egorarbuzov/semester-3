interface IFlyable
{
    void Fly();
}

interface ISwimmable
{
    void Swim();
}

interface IRunnable
{
    void Run();
}

class Duck : IFlyable, ISwimmable, IRunnable
{
    public string Name { get; set; }

    public Duck(string name)
    {
        this.Name = name;
    }

    public void Fly()
    {
        Console.WriteLine($"{this.Name} летит");
    }

    public void Swim()
    {
        Console.WriteLine($"{this.Name} плывёт");
    }

    public void Run()
    {
        Console.WriteLine($"{this.Name} бежит");
    }
}

class Fish : ISwimmable
{
    public string Name { get; set; }

    public Fish(string name)
    {
        this.Name = name;
    }

    public void Swim()
    {
        Console.WriteLine($"{this.Name} плывёт");
    }
}

class Dog : IRunnable, ISwimmable
{
    public string Name { get; set; }

    public Dog(string name)
    {
        this.Name = name;
    }

    public void Run()
    {
        Console.WriteLine($"{this.Name} бежит");
    }

    public void Swim()
    {
        Console.WriteLine($"{this.Name} плывёт");
    }
}

class Program
{
    static void Main(string[] args)
    {
        List<object> animals = new List<object>();
        animals.Add(new Duck("Утка"));
        animals.Add(new Fish("Рыба"));
        animals.Add(new Dog("Собака"));

        int flyCount = 0;
        int swimCount = 0;
        int runCount = 0;

        foreach (object animal in animals)
        {
            IFlyable flyable = animal as IFlyable;
            if (flyable != null)
            {
                flyable.Fly();
                flyCount++;
            }

            ISwimmable swimmable = animal as ISwimmable;
            if (swimmable != null)
            {
                swimmable.Swim();
                swimCount++;
            }

            IRunnable runnable = animal as IRunnable;
            if (runnable != null)
            {
                runnable.Run();
                runCount++;
            }
        }

        Console.WriteLine($"Умеют летать: {flyCount}");
        Console.WriteLine($"Умеют плавать: {swimCount}");
        Console.WriteLine($"Умеют бегать: {runCount}");
    }
}