import csv
import datetime
import requests

# Function to get PMS data from Mobius
def get_pms_data():
    url = "http://203.253.128.177:7579/Mobius/20191546/PMS/la"
    
    headers = {
        'Accept': 'application/json',
        'X-M2M-RI': '12345',
        'X-M2M-Origin': '20191546',
        'Content-Type': 'application/vnd.onem2m-res+json; ty=4'
    }
    
    r = requests.get(url, headers=headers)
    
    try:
        r.raise_for_status()
        jr = r.json()
        return jr["m2m:cin"]["con"]
    except Exception as exc:
        print('There was a problem: %s' % (exc))

# Main function to save data to CSV file
def main():
    # Get PMS data from Mobius
    pms_data = get_pms_data()
    
    if pms_data:
        # Get the current time
        current_time = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
        
        # Create a list with the data to be written to the CSV file
        csv_data = [current_time, pms_data]
        
        # Write the data to the CSV file in a specific directory
        file_path = r'C:\Users\kksp1\OneDrive\Desktop\메이커톤\mobius\pms_data.csv'
        with open(file_path, 'a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(csv_data)
        print("Data saved to pms_data.csv")

if __name__ == "__main__":
    main()
