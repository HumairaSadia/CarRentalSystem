#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include<bits/stdc++.h>
using namespace std;

class User
{
protected:
    string username;
    string password;

public:
    User(string username, string password) : username(username), password(password) {}

    virtual void displayProfile() const
    {
        cout << "Username: " << username << endl;
    }

    friend class CarRentalSystem;
};

class Owner : public User
{
private:
    string companyName;

public:
    Owner(string username, string password, string companyName) : User(username, password), companyName(companyName) {}

    void displayProfile() const override
    {
        cout << "Owner Profile:" << endl;
        User::displayProfile();
        cout << "Company Name: " << companyName << endl;
    }

    friend class CarRentalSystem;
};

class Customer : public User
{
public:
    Customer(string username, string password) : User(username, password) {}

    void displayProfile() const override
    {
        cout << "Customer Profile:" << endl;
        User::displayProfile();
    }

    friend class CarRentalSystem;
};

class Car
{
private:
    string brand;
    string model;
    string licenseNo;
    bool available;

public:
    Car(string brand, string model, string licenseNo)
        : brand(brand), model(model), licenseNo(licenseNo), available(true) {}

    string getbrand() const
    {
        return brand;
    }
    string getModel() const
    {
        return model;
    }
    string getlicenseNo() const
    {
        return licenseNo;
    }
    bool isAvailable() const
    {
        return available;
    }

    void setAvailability(bool availability)
    {
        available = availability;
    }
};

class CarRentalSystem
{
private:
    vector<Car> cars;
    vector<Owner> owners;
    vector<Customer> customers;

    string carFileName = "cars.txt";
    string ownerFileName = "owners.txt";
    string customerFileName = "customers.txt";

public:
    CarRentalSystem()
    {
        loadCarsFromFile();
        loadOwnersFromFile();
        loadCustomersFromFile();
    }

    void loadCarsFromFile()
    {
        ifstream inFile(carFileName);
        string brand, model, licenseNo;
        bool available;

        if (inFile.is_open())
        {
            while (inFile >> brand >> model >> licenseNo >> available)
            {
                cars.push_back(Car(brand, model, licenseNo));
                cars.back().setAvailability(available);
            }
            inFile.close();
        }
    }

    void saveCarsToFile()
    {
        ofstream outFile(carFileName);
        if (outFile.is_open())
        {
            for (const auto& car : cars)
            {
                outFile << car.getbrand() << " " << car.getModel() << " " << car.getlicenseNo() << " " << car.isAvailable() << endl;
            }
            outFile.close();
        }
    }

    void loadOwnersFromFile()
    {
        ifstream inFile(ownerFileName);
        string username, password, companyName;

        if (inFile.is_open())
        {
            while (inFile >> username >> password >> companyName)
            {
                owners.push_back(Owner(username, password, companyName));
            }
            inFile.close();
        }
    }

    void saveOwnersToFile()
    {
        ofstream outFile(ownerFileName);
        if (outFile.is_open())
        {
            for (const auto& owner : owners)
            {
                outFile << owner.username << " " << owner.password << " " << owner.companyName << endl;
            }
            outFile.close();
        }
    }

    void loadCustomersFromFile()
    {
        ifstream inFile(customerFileName);
        string username, password;

        if (inFile.is_open())
        {
            while (inFile >> username >> password)
            {
                customers.push_back(Customer(username, password));
            }
            inFile.close();
        }
    }

    void saveCustomersToFile()
    {
        ofstream outFile(customerFileName);
        if (outFile.is_open())
        {
            for (const auto& customer : customers)
            {
                outFile << customer.username << " " << customer.password << endl;
            }
            outFile.close();
        }
    }

    bool ownerLogin(const string& username, const string& password) const
    {
        for (const auto& owner : owners)
        {
            if (owner.username == username && owner.password == password)
            {
                return true;
            }
        }
        return false;
    }

    bool customerLogin(const string& username, const string& password) const
    {
        for (const auto& customer : customers)
        {
            if (customer.username == username && customer.password == password)
            {
                return true;
            }
        }
        return false;
    }

    void registerOwner(const Owner& owner)
    {
        owners.push_back(owner);
        saveOwnersToFile();
    }

    void registerCustomer(const Customer& customer)
    {
        customers.push_back(customer);
        saveCustomersToFile();
    }

    void addCar(const Car& car)
    {
        cars.push_back(car);
        saveCarsToFile();
    }

    void showAvailableCars() const
    {
        cout << "Available Cars: " << endl;
        cout << setw(5) << "Serial" << setw(10) << "brand" << setw(10) << "Model" << setw(15) << "License Plate" << endl;
        int serial = 1;
        for (const auto& car : cars)
        {
            if (car.isAvailable())
            {
                cout << setw(5) << serial << setw(10) << car.getbrand() << setw(10) << car.getModel() << setw(15) << car.getlicenseNo() << endl;
                ++serial;
            }
        }
    }

    void showRentedCars() const
    {
        cout << "Rented Cars: " << endl;
        cout << setw(5) << "Serial" << setw(10) << "brand" << setw(10) << "Model" << setw(15) << "License Plate" << endl;
        int serial = 1;
        for (const auto& car : cars)
        {
            if (!car.isAvailable())
            {
                cout << setw(5) << serial << setw(10) << car.getbrand() << setw(10) << car.getModel() << setw(15) << car.getlicenseNo() << endl;
                ++serial;
            }
        }
    }

    bool selectCar(int serialNumber)
    {
        if (serialNumber > 0 && serialNumber <= cars.size())
        {
            if (cars[serialNumber - 1].isAvailable())
            {
                cout << "You have selected the following car: " << endl;
                cout << "brand: " << cars[serialNumber - 1].getbrand() << endl;
                cout << "Model: " << cars[serialNumber - 1].getModel() << endl;
                cout << "License Plate: " << cars[serialNumber - 1].getlicenseNo() << endl;
                cars[serialNumber - 1].setAvailability(false);
                saveCarsToFile();
                return true;
            }
            else
            {
                cout << "Car is not available. Please select another car." << endl;
                return false;
            }
        }
        else
        {
            cout << "Invalid serial number." << endl;
            return false;
        }
    }

    bool returnCar(int serialNumber)
    {
        if (serialNumber > 0 && serialNumber <= cars.size())
        {
            if (!cars[serialNumber - 1].isAvailable())
            {
                cout << "You have returned the following car: " << endl;
                cout << "brand: " << cars[serialNumber - 1].getbrand() << endl;
                cout << "Model: " << cars[serialNumber - 1].getModel() << endl;
                cout << "License Plate: " << cars[serialNumber - 1].getlicenseNo() << endl;
                cars[serialNumber - 1].setAvailability(true);
                saveCarsToFile();
                return true;
            }
            else
            {
                cout << "Car is already available. Please return another car." << endl;
                return false;
            }
        }
        else
        {
            cout << "Invalid serial number." << endl;
            return false;
        }
    }
};

int main()
{
    CarRentalSystem System;
    int choice;
    string username, password;

    do
    {
        cout << "\nCAR RENTAL System MENU:" << endl;
        cout<<endl;
        cout << "1. Register as Owner" << endl;
        cout << "2. Register as Customer" << endl;
        cout << "3. Login as Owner" << endl;
        cout << "4. Login as Customer" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        system("cls");
        switch (choice)
        {
        case 1:

            L:
            cout<<"==========OWNER'S PROFILE=========="<<endl;
            cout<<endl;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (System.ownerLogin(username, password))
            {
                cout<<"Username and Password Already Exist\n";
                goto L;
            }
            else
            {
                System.registerOwner(Owner(username, password, ""));
                cout << "Owner registered successfully!" << endl;
            }
            break;
        case 2:
            cout<<"==========CUSTOMER'S PROFILE=========="<<endl;
            cout<<endl;
            C:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (System.customerLogin(username, password))
            {
                cout<<"Username and Password Already Exist\n";
                goto C;
            }
            System.registerCustomer(Customer(username, password));
            cout << "Customer registered successfully!" << endl;
            break;
        case 3:
            cout<<"==========OWNER'S PROFILE=========="<<endl;
            cout<<endl;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (System.ownerLogin(username, password))
            {
                cout << "Owner login successful!" << endl;
                string brand, model, licenseNo;
                cout << "Enter car brand: ";
                cin >> brand;
                cout << "Enter car model: ";
                cin >> model;
                cout << "Enter car license plate: ";
                cin >> licenseNo;
                System.addCar(Car(brand, model, licenseNo));
                cout << "Car registered successfully for rent!" << endl;
            }
            else
            {
                cout << "Invalid username or password. Please try again." << endl;
            }
            break;
        case 4:
            cout<<"==========CUSTOMER'S PROFILE=========="<<endl;
            cout<<endl;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (System.customerLogin(username, password))
            {
                cout << "Customer login successful!" << endl;
                int option;
                do
                {
                    cout << "\nCUSTOMER MENU:" << endl;
                    cout<<endl;
                    cout << "1. View Available Cars" << endl;
                    cout << "2. Select a Car" << endl;
                    cout << "3. Return a Car" << endl;
                    cout << "4. Exit" << endl;
                    cout << "Enter your choice: ";
                    cin >> option;
                    system("cls");

                    switch (option)
                    {
                    case 1:
                        System.showAvailableCars();
                        break;
                    case 2:
                        int serialNumber;
                        cout << "Enter the serial number of the car you want to select: ";
                        cin >> serialNumber;
                        System.selectCar(serialNumber);
                        break;
                    case 3:
                        cout << "Cars you rented: " << endl;
                        System.showRentedCars();
                        cout << "Enter the serial number of the car you want to return: ";
                        cin >> serialNumber;
                        System.returnCar(serialNumber);
                        break;
                    case 4:
                        cout << "Exiting customer menu and back to car rental System menu." << endl;
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                    }
                }
                while (option != 4);
            }
            else
            {
                cout << "Invalid username or password. Please try again." << endl;
            }
            break;
        case 5:
            cout << "EXITING PROGRAME." << endl;
            cout<<endl;
            cout<< "==========THANK YOU=========="<<endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
    while (choice != 5);

    return 0;
}
