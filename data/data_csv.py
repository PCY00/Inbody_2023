import json
import csv
import datetime

# 주어진 JSON 데이터
data_json = '''

'''

# JSON 데이터를 Python 리스트로 변환
data = json.loads(data_json)

# 밀리초를 초로 변환하는 함수
def convert_to_seconds(milliseconds):
    seconds = milliseconds // 1000
    return seconds

# 유닉스 타임스탬프를 년, 월, 일, 시, 분, 초로 변환하는 함수
def timestamp_to_datetime(timestamp):
    dt_object = datetime.datetime.fromtimestamp(timestamp)
    return dt_object

# 날짜를 스트링 형태로 변환하는 함수
def timestamp_to_string(timestamp):
    dt_object = datetime.datetime.fromtimestamp(timestamp)
    return dt_object.strftime('%Y%m%d%H%M%S')

# CSV 파일에 저장
csv_file = "data.csv"
with open(csv_file, 'w', newline='') as file:
    fieldnames = ['date_time', 'heart_rate']
    writer = csv.DictWriter(file, fieldnames=fieldnames)

    writer.writeheader()
    for item in data:
        item["start_time"] = convert_to_seconds(item["start_time"])
        item["date_time"] = timestamp_to_string(item["start_time"])
        writer.writerow({
            'date_time': item['date_time'],
            'heart_rate': item['heart_rate']
        })

print("데이터가 CSV 파일에 저장되었습니다.")
