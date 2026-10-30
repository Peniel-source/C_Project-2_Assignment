#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int id;
    char name[40];
    int type;                   
    union {
        float temperature;
        int pressure;
        float voltage;
    } readings;
    int status;                 
    float prev;         //previous reading         
    int count;          //number of readings a device generates  
    char cbname[32];     //callback name. the callback that raises the alert      
    char reason[40];     //why the alert was raised
    float lo;          //lower limit         
    float hi;          //upper limit         
} Device;

Device *devs = NULL;      //array of devices
int n = 0;                // number of devices  

//these are counters for the summary
int total = 0, normal = 0, abnormal = 0, drifts = 0;
int alerts[3] = {0, 0, 0};  

const char *tname[3] = {"Temperature", "Pressure", "Battery"};
const char *unit[3] = {"C", "kPa", "V"};


//my function prototypes
float get_value(Device *d);
void print_value(Device *d);
void set_alert(Device *d, const char *cb, float low, float high, const char *why);
void show_alert(Device *d);
void process_device(Device *device, void (*callback)(Device *));
void temperature_monitor(Device *d);
void pressure_monitor(Device *d);
void battery_monitor(Device *d);
void sensor_drift_detector(Device *d);
void register_device(void);
void display_devices(void);
void remove_device(void);
void generate_readings(void);
void process_readings(void);
void display_alerts(void);
void monitoring_summary(void);

int main(void) {
    int choice;
    srand(time(NULL));
    while (1) {
        printf("\n1.Register device\n2.Display devices\n3.Remove device\n4.Generate readings\n5.Process readings\n6.Display alerts\n7.Monitoring summary\n8.Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1 || choice == 0)
            break;
        switch (choice) {
            case 1: 
                register_device(); 
                break;
            case 2: 
                display_devices(); 
                break;
            case 3: 
                remove_device(); 
                break;
            case 4: 
                generate_readings(); 
                break;
            case 5: 
                process_readings(); 
                break;
            case 6: 
                display_alerts(); 
                break;
            case 7: 
                monitoring_summary(); 
                break;
            default: 
                printf("wrong choice\n");
        }
    }
    free(devs);
    return 0;
}

//this funx returns the reading as a float. The device type decides which union member is read
float get_value(Device *d) {
    if (d->type == 0) return d->readings.temperature;
    if (d->type == 1) return (float)d->readings.pressure;
    return d->readings.voltage;
}

//print the reading with its unit
void print_value(Device *d) {
    printf("%.*f %s", d->type == 1 ? 0 : 2, get_value(d), unit[d->type]);
}

//this sets a device in alert and records the details from show_alert
void set_alert(Device *d, const char *cb, float low, float high, const char *why) {
    d->status = 1;
    strcpy(d->cbname, cb);
    strcpy(d->reason, why);
    d->lo = low;
    d->hi = high;
}

//print one alert
void show_alert(Device *d) {
    printf("ALERT id=%d %s: ", d->id, d->name);
    print_value(d);
    printf("expected: %.1f to %.1f %s reason: %s callback: %s\n", d->lo, d->hi, unit[d->type], d->reason, d->cbname);
}

//this function runs a callback on device
void process_device(Device *device, void (*callback)(Device *)) {
    if (device != NULL && callback != NULL)
        callback(device);
}

//callback for temperature
void temperature_monitor(Device *d) {
    float t = d->readings.temperature;
    if (t < 15 || t > 70)
        set_alert(d, "temperature_monitor", 15, 70, t < 15 ? "too cold" : "too hot");
}

//callback for pressure
void pressure_monitor(Device *d) {
    int p = d->readings.pressure;
    if (p < 70 || p > 130)
        set_alert(d, "pressure_monitor", 70, 130, p < 70 ? "pressure too low" : "pressure too high");
}

//callback for battery
void battery_monitor(Device *d) {
    float v = d->readings.voltage;
    if (v < 3.0 || v > 4.2)
        set_alert(d, "battery_monitor", 3.0, 4.2, v < 3.0 ? "battery low" : "overvoltage");
}
  

//sensor drift detector
//this functions catches a sudden changes between two readings on the same device
//logic: the allowed range is previous reading x 0.65 up to previous reading x 1.35
//if the reading is outside the range,it is a drift alert
void sensor_drift_detector(Device *d) {
    float v = get_value(d);
    float low = d->prev * 0.65, high = d->prev * 1.35;
    if (d->count < 2) 
        return;
    if (v < low || v > high) {
        drifts++;
        set_alert(d, "sensor_drift_detector", low, high, "changes from previous reading");
    }
}

//add a device
void register_device(void) {
    Device *d, *new_mem;
    int id = 0, type = 0;
    printf("ID: ");
    scanf("%d", &id);

    if (id <= 0) 
    { 
        printf("Id is not valid\n"); 
        return; 
    }

    for (d = devs; d < devs + n; d++)
        if (d->id == id) 
        { 
            printf("THis Id already exist.\n"); 
            return; 
        }
    printf("Type 1.Temperature 2.Pressure 3.Battery: ");
    scanf("%d", &type);
    if (type < 1 || type > 3) 
    { 
        printf("Invalid type\n"); 
        return; 
    }
    //re allocate memomru to n + 1 devices
    new_mem = realloc(devs, (n + 1) * sizeof *devs);    
    if (new_mem == NULL) 
    { 
        printf("failed to allocate memory\n"); 
        return; 
    }
    devs = new_mem;
    d = devs + n;  //this is the new slot in the array

    //initialize the fields
    d->id = id;
    d->type = type - 1;
    d->status = 0;
    d->count = 0;
    d->prev = 0.0f;
    d->cbname[0] = '\0';
    d->reason[0] = '\0';
    d->lo = 0.0f;
    d->hi = 0.0f;

    printf("Name: ");
    scanf("%39s", d->name);  //no more than 39 characters
    n++;
    printf("Device sucessfuly registered\n");
}


//show all devices in a table 
void display_devices(void) {
    Device *d;
    if (n == 0) 
    { 
        printf("No devices.\n"); 
        return; 
    }
    printf("%-5s %-12s %-12s %-12s %s\n", "ID", "Name", "Type", "Reading", "Status");
    for (d = devs; d < devs + n; d++) 
    {
        printf("%-5d %-12s %-12s ", d->id, d->name, tname[d->type]);
        
        if (d->count == 0) {
            printf("- ");
        } else {
            print_value(d);
            printf(" ");
        }

        if (d->status == 1) 
            printf("ALERT\n");
        else if (d->status == -1) 
            printf("NEW\n");
        else 
            printf("OK\n");
    }
}

//remove a device by id
void remove_device(void) {
    Device *d, *new_mem;
    int id = 0;
    printf("Enter device Id: ");
    scanf("%d", &id);
    for (d = devs; d < devs + n && d->id != id; d++)
        ;
    if (d == devs + n) 
    { 
        printf("This device cannot be found\n"); 
        return; 
    }

    for (; d < devs + n - 1; d++)
        *d = *(d + 1);

    n--;
    if (n == 0) {
        free(devs);
        devs = NULL;
    } else {
        new_mem = realloc(devs, n * sizeof *devs);        
        if (new_mem != NULL) 
            devs = new_mem;
    }
    printf("Device has been removed\n");
}

//this function generates random readings for the devices
void generate_readings(void) {
    Device *d;
    float r;
    for (d = devs; d < devs + n; d++) {
        if (d->count > 0) 
            d->prev = get_value(d);        
        r = rand() / (float)RAND_MAX;                  
        if (d->type == 0) {
            d->readings.temperature = 10 + 70 * r;
        }
        else if (d->type == 1) {
            d->readings.pressure = 50 + (int)(100 * r);
        }
        else {
            d->readings.voltage = 2.5 + 2.0 * r;
        }
        d->count++;
        d->status = -1;                                  
    }
    printf("Generated %d readings\n", n);
}

void process_readings(void) {
    void (*monitor[3])(Device *) = {temperature_monitor, pressure_monitor, battery_monitor};
    Device *d;
    for (d = devs; d < devs + n; d++) {
        if (d->status != -1) 
            continue;                   
        d->status = 0;
        total++;
        process_device(d, monitor[d->type]);             
        if (d->status == 0)
            process_device(d, sensor_drift_detector);    
        printf("id=%d %s %s ", d->id, d->name, tname[d->type]);
        print_value(d);
        if (d->status == 1) {
            abnormal++;
            alerts[d->type]++;
            printf("Abnormal\n");
            show_alert(d);
        } else {
            normal++;
            printf("Normal\n");
        }
    }
}

void display_alerts(void) {
    Device *d;
    int found = 0;
    for (d = devs; d < devs + n; d++)
        if (d->status == 1) 
        { 
            show_alert(d); 
            found = 1; 
        }
    if (!found) 
        printf("No devices in alert\n");
}

void monitoring_summary(void) {
    printf("Registered devices: %d\n", n);
    printf("Processed readings: %d\n", total);
    printf("Normal readings: %d\n", normal);
    printf("Abnormal readings: %d\n", abnormal);
    printf("Alerts - Temperature: %d, Pressure: %d, Battery: %d\n", alerts[0], alerts[1], alerts[2]);
    printf("Alert drifts: %d\n", drifts);
}
