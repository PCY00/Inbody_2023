# Inbody_2023
---
### 설명
논문을 토대로 개인의 최적의 휴식시간을 제공해준다.
기대효과로는 운동강도도 체크하여 추후 운동강도가 낮을 경우 운동을 강도에 대한 조언을 해줄 수 있다.
DeepLearning은 1주간 실제 3명의 참여자가 갤럭시워치4를 이용하여 휴식 시간 및 심박도의 데이터를 얻은 것과
얻은 것의 패턴을 분석하여 1만개의 난수를 생성한 것을 합쳐서 학습시킨다.

학습하여 얻은 최적의 휴식시간을 IoT서버인 모비우스에 전송하고 전송된 휴식 시간을 가져와 APP으로 전송해준다.
(단, 학습한 휴식시간은 전날 데이터를 활용하여 답은 낸 휴식시간이다.)
APP에선 운동 시작 버튼을 눌러 운동을 시작하고 논문과 동일 조건으로 일정 템포 벤치프레스 1rm 50%로 10개를 진행하게 하고
전송 받은 휴식 시간을 카운터 해준다.

템포는 APP에서 소리를 활용하여 사용자가 템포를 유지하게끔 하였다.

프로토타입으로 운동 밴드를 만들었고, 화면에는 실시간 심박데이터를 보여준다.

추후 APP에서 진행하는 것들을 운동 밴드에서 작동하도록 만들계획이다.
