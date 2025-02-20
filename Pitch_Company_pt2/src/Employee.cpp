#include "Employee.h"
#include <fstream>

using namespace std;

/* -------------------- EMPLOYEE -------------------- */

Employee::Employee(string name, unsigned int nif, unsigned int age) {
    this->name = name;
    this->nif = nif;
    this->age = age;
    function = "general employee";
    evaluation = 0;
    numReviews = 0;
}

string Employee::show() {
    stringstream info;
    info << "NIF: " << nif << '\n'
         << "NAME: " << name << '\n'
         << "FUNCTION: " << function << '\n'
         << "EVALUATION: " << evaluation << "   NUMBER OF REVIEWS: " << numReviews;
    return info.str();
}

void Employee::printToFile(ofstream &op) {
    op << name << '\n'
       << nif << '\n'
       << age << '\n'
       << evaluation << " " << numReviews << '\n';
}

unsigned int Employee::getNif() const {
    return nif;
}

unsigned int Employee::getEvaluation() const {
    return evaluation;
}

unsigned int Employee::getNumOfReviews() const {
    return numReviews;
}

void Employee::setOldReviews(unsigned int eva, unsigned int nR) {
    if (eva < 0)
        eva = 0;
    if (eva > 5)
        eva = 5;
    this->evaluation = eva;
    this->numReviews = nR;
}

void Employee::review(unsigned int punctuation) {
    evaluation = (evaluation * numReviews) + punctuation;
    numReviews++;
    evaluation = evaluation / numReviews;
}

bool Employee::operator<(Employee *comp) {
    if (this->function == "maintenance" && comp->function != "lift manager")
        return false;
    else if (this->function != "maintenance" && comp->function == "lift manager")
        return false;
    if (this->function == "lift manager" && comp->function == "lift manager")
        return this->evaluation < comp->evaluation;
    return dynamic_cast<ServiceProvider *>(this)->getNumMaintenancesDone() >
           dynamic_cast<ServiceProvider *>(comp)->getNumMaintenancesDone();

}

bool Employee::operator==(Employee *comp) {
    return (nif == comp->nif);
}



/* -------------------- SERVICE PROVIDER -------------------- */

ServiceProvider::ServiceProvider(string name, unsigned int nif, unsigned int age, double perHourSalary, unsigned int numMaintenances) : Employee(name, nif, age) {
    function = "maintenance";
    hoursWorked = 0;
    numMaintenances = 0;
    this->perHourSalary = perHourSalary;
}

string ServiceProvider::show() {
    stringstream info;
    info << "NIF: " << nif << '\n'
         << "NAME: " << name << '\n'
         << "FUNCTION: " << function << '\n'
         << "EVALUATION: " << evaluation << "   NUMBER OF REVIEWS: " << numReviews << '\n'
         << "PER HOUR: " << perHourSalary << "   HOURS: " << hoursWorked << "   TOTAL TO RECEIVE: " << getSalary()
         << '\n'
         << "MAINTENANCES EXECUTED: " << numMaintenances << "   MAINTENANCES TO EXECUTE: " << schedule.size();
    return info.str();
}

void ServiceProvider::printToFile(ofstream &op) {
    op << name << '\n'
       << nif << '\n'
       << age << '\n'
       << evaluation << " " << numReviews << '\n'
       << "+\n"
       << setprecision(3) << perHourSalary << " " << setprecision(2) << hoursWorked << " " << numMaintenances << '\n';
    if (schedule.empty()) {
        op << "--------------------\n--------------------\n";
    } else { //prints service provider schedule
        heapMaint aux = schedule;
        op << "--------------------\n";
        while (!aux.empty()) {
            aux.top()->printToEmployeesFile(op);
            op << '\n';
            aux.pop();
        }
        op << "--------------------\n";
    }
    op << "+\n";
}

double ServiceProvider::getSalary() const {
    return (perHourSalary * hoursWorked);
}

unsigned int ServiceProvider::getNumAppointments() const {
    return schedule.size();
}

heapMaint ServiceProvider::getSchedule() const {
    return schedule;
}

int ServiceProvider::getNumMaintenancesDone() const {
    return numMaintenances;
}

void ServiceProvider::setHoursWorked(double h) {
    hoursWorked = h;
}

void ServiceProvider::setNumMaintenances(unsigned int num) {
    numMaintenances = num;
}

bool ServiceProvider::scheduleMaintenance(Maintenance *m) {
    schedule.push(m);
    return true;
}

void ServiceProvider::executeMaintenance() {
    hoursWorked += MAINTDURATION;
    numMaintenances++;
    schedule.pop();
}

bool ServiceProvider::operator<(Employee *comp) {
    ServiceProvider *sp = dynamic_cast<ServiceProvider *>(comp);
    if (comp != NULL) {
        if (this->numMaintenances > sp->getNumMaintenancesDone()) return true;
        else if (numMaintenances == sp->getNumMaintenancesDone() && evaluation > sp->getEvaluation()) return true;
        return false;
    } else {
        return false;
    }
}

bool ServiceProvider::operator==(Employee *comp) {
    ServiceProvider *sp = dynamic_cast<ServiceProvider *>(comp);
    if (comp != NULL) {
        return (nif == sp->nif);
    } else {
        return false;
    }
}



/* -------------------- SERVICE PROVIDER TYPE -------------------- */

ServiceProviderType::ServiceProviderType(string name, unsigned int nif, unsigned int age, double perHourSalary, unsigned int numMaintenances) {
    ServiceProvider *serviceProvider = new ServiceProvider(name, nif, age, perHourSalary, numMaintenances);
    sp = serviceProvider;
}

ServiceProviderType::ServiceProviderType(ServiceProvider *sp) {
    this->sp = sp;
}

ServiceProvider * ServiceProviderType::getServiceProvider() const {
    return sp;
}

bool ServiceProviderType::operator<(const ServiceProviderType toCmp) const {
    if (sp->getNumMaintenancesDone() > toCmp.getServiceProvider()->getNumMaintenancesDone()) return true;
    else if (sp->getNumMaintenancesDone() == toCmp.getServiceProvider()->getNumMaintenancesDone() && sp->getEvaluation() < toCmp.getServiceProvider()->getEvaluation()) return true;
    return false;
}

bool ServiceProviderType::operator==(const ServiceProviderType toCmp) const {

    return (sp->getNif() == toCmp.getServiceProvider()->getNif());

}



/* -------------------- CONTRACTED EMPLOYEE -------------------- */

ContractedEmployee::ContractedEmployee(string name, unsigned int nif, unsigned int age, double perMonthSalary) : Employee(name, nif, age) {
    function = "lift manager";
    this->perMonthSalary = perMonthSalary;
}

string ContractedEmployee::show() {
    stringstream info;
    info << "NIF: " << nif << '\n'
         << "NAME: " << name << '\n'
         << "FUNCTION: " << function << '\n'
         << "EVALUATION: " << evaluation << "   NUMBER OF REVIEWS: " << numReviews << '\n'
         << "SALARY: " << perMonthSalary << '\n'
         << "NUMBER OF CLIENTS ASSOCIATED: " << getNumClientsAssociated() << '\n';
    return info.str();
}

void ContractedEmployee::printToFile(ofstream &op) {
    op << name << '\n'
       << nif << '\n'
       << age << '\n'
       << evaluation << " " << numReviews << '\n'
       << "x\n"
       << setprecision(4) << perMonthSalary << "\n";
    if (clientsAssociated.size() == 0) {
        op << 0;
    } else {
        for (vector<Client *>::iterator it = clientsAssociated.begin(); it != clientsAssociated.end(); it++) {
            op << (*it)->getNif() << " ";
        }
    }
    op << '\n'
       << "x\n";
}

unsigned int ContractedEmployee::getNumClientsAssociated() const {
    return clientsAssociated.size();
}

vector<Client *> ContractedEmployee::getClientsAssociated() {
    return clientsAssociated;
}

void ContractedEmployee::associateToClient(Client *c) {
    clientsAssociated.push_back(c);
}

void ContractedEmployee::removeClient(Client *rm) {
    for (vector<Client *>::iterator it = clientsAssociated.begin(); it != clientsAssociated.end(); it++) {
        if (*it == rm) {
            clientsAssociated.erase(it);
            break;
        }
    }
}

bool ContractedEmployee::operator<(Employee *comp) {
    ContractedEmployee *ce = dynamic_cast<ContractedEmployee *>(comp);
    if (comp != NULL) {
        if (evaluation < ce->evaluation) return true;
        else if (evaluation == ce->evaluation && clientsAssociated.size() > ce->clientsAssociated.size()) return true;
        return false;
    } else {
        return false;
    }
}

bool ContractedEmployee::operator==(Employee *comp) {
    ContractedEmployee *ce = dynamic_cast<ContractedEmployee *>(comp);
    if (comp != NULL) {
        return (nif == ce->nif);
    } else {
        return false;
    }
}



/* -------------------- CONTRACTED EMPLOYEE TYPE -------------------- */

ContractedEmployeeType::ContractedEmployeeType(string name, unsigned int nif, unsigned int age, double perMonthSalary) {
    ContractedEmployee *contractedEmployee = new ContractedEmployee(name, nif, age, perMonthSalary);
    ce = contractedEmployee;
}

ContractedEmployeeType::ContractedEmployeeType(ContractedEmployee *ce) {
    this->ce = ce;
}

ContractedEmployee * ContractedEmployeeType::getContractedEmployee() const {
    return ce;
}

bool ContractedEmployeeType::operator<(const ContractedEmployeeType toCmp) const {
    if (ce->getEvaluation() < toCmp.getContractedEmployee()->getEvaluation()) return true;
    else if (ce->getEvaluation() == toCmp.getContractedEmployee()->getEvaluation() && ce->getNumClientsAssociated() > toCmp.getContractedEmployee()->getNumClientsAssociated()) return true;
    return false;
}

bool ContractedEmployeeType::operator==(const ContractedEmployeeType toCmp) const {
    return (ce->getNif() == toCmp.getContractedEmployee()->getNif());
}




ostream &operator<<(ostream &out, Employee *e) {
    out << e->show();
    return out;
}


bool sortEmployeesByNumReviews(Employee *e1, Employee *e2) {
    if (e2->getNumOfReviews() > e1->getNumOfReviews()) return true;
    else return false;
}

bool sortEmployeesByNumClientsAssociated(Employee *e1, Employee *e2) {
    ContractedEmployee *ce1 = dynamic_cast<ContractedEmployee *>(e1);
    ContractedEmployee *ce2 = dynamic_cast<ContractedEmployee *>(e2);
    if (ce1 != NULL & ce2 != NULL) {
        if (ce2->getNumClientsAssociated() > ce1->getNumClientsAssociated()) return true;
        else return false;
    }
    return false;
}

