class Animal
{
    public Animal(string name, int age, double weight)
    {
        this.Name = name;
        this.Age = age;
        this.Weight = weight;
    }
    public string Name { get; set; }
    public int Age { get; set; }
    public double Weight { get; set; }

    public virtual void MakeSound(bool isDay)
    {
        Console.WriteLine($"{Name} издает звук");
    }
}

class Dog : Animal
{
    public Dog(string name, int age, double weight) : base(name, age, weight) { }
    
    public override void MakeSound(bool isDay)
    {
        if (isDay)
        {
            Console.WriteLine($"{Name} : гав-гав-гав!");
        }
        else
        {
            Console.WriteLine($"{Name} : хр-хр-хр");
        }
    }
}

class Cat : Animal {
    public Cat(string name, int age, double weight) : base(name, age, weight) { }

    public override void MakeSound(bool isDay)
    {
        if (isDay) { Console.WriteLine($"{Name} : мяу-мяу-мяу"); }
        else
        {
            Console.WriteLine($"{Name} : хр-хр-хр ммм");
        }

    }
}

class Program
{
    static void Main(string[] args)
    {
        Animal a = new Animal("Лев", 10, 70.4);
        Dog b = new Dog("Бобик", 12, 1.2);
        Cat c = new Cat("Мяу", 8, 2.5);
        
        a.MakeSound(false);
        b.MakeSound(true);
        c.MakeSound(false);
    }
}