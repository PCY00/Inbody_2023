import requests
import time

# Mobius에 데이터를 전송하는 함수
def send_onem2m_data(data):
    url = "http://203.253.128.177:7579/Mobius/20191546/PMS"
    
    headers = {
        'Accept': 'application/json',
        'X-M2M-RI': '12345',
        'X-M2M-Origin': '20191546',
        'Content-Type': 'application/vnd.onem2m-res+json; ty=4'
    }
    
    data = {
        "m2m:cin": {
            "con" : data
        }
    }
    
    r = requests.post(url, headers=headers, json=data)
    
    try:
        r.raise_for_status()
        jr = r.json()
        print(jr['m2m:cin']['con'])
    except Exception as exc:
        print('전송 중 오류 발생: %s' % (exc))

def main():
    try:
        value = 0

        while True:
            # 값을 1씩 증가시킴
            value += 1

            # 증가된 값을 문자열로 변환하여 Mobius에 전송
            send_onem2m_data(str(value))

            # 5초 대기
            time.sleep(5)

    except KeyboardInterrupt:
        print("사용자에 의해 스크립트가 종료되었습니다.")

if __name__ == "__main__":
    main()
