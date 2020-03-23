# Data-collector
연구실에서 아두이노로 개발한 뮤온 디텍터의 데이터 수집을 위해 간단한 GUI 프로그램을 만들게 되었습니다. 
아두이노 뿐만이 아니라, PC (Windows 운영체제에서만 사용 가능)의 해당 시리얼 포트로 들어오는 데이터를 수집하여 폴더에 저장해주는 간단한 프로그램입니다.
MCU를 통해 조그마한 개인 프로젝트를 진행하시는 분들께서 Serial communication 동작을 확인하시고자 혹은 자동적인 데이터 수집을 원하시는 경우 해당 프로그램이 큰 도움이 될 것이라 생각합니다.

![캡처](https://user-images.githubusercontent.com/54430715/77289992-7f469500-6d1e-11ea-9673-4d6cc1b13ea7.PNG)

- 사용방법
1) Port scan 버튼을 눌러 사용가능한 포트 검색
2) 콤보박스에서 데이터 수집을 원하는 포트 선택
3) 저장 옵션 선택  
   "Save as...": 데이터 저장 Path 및 파일명 설정 후 해당 파일에 데이터를 저장하기 시작함. 시작 후 한 번 더 클릭할 시 데이터 저장이 종료됨  
   "Auto save (Every hour)": 매 시간 마다 데이터 저장 파일을 자동으로 생성하며 데이터를 저장함. 시작 후 한 번 더 클릭할 시 데이터 저장이 종료됨. 데이터가 저장되는 위치는 "...\Documents\DATACOLLECTOR_SAVEFILES"로 설정되어 있음
4) Start 버튼을 누르면 해당 포트와 연결되며 그 순간부터 들어오는 모든 데이터를 .txt 형식의 파일에 저장함. 마찬가지로 Stop을 누를 경우 해당 포트와 연결이 해제되며 데이터 수집이 중단됨 (들어오는 문자열에 개행문자('\n')가 포함되어있는 경우 자동 줄바꿈되어 데이터를 저장함)

*데이터가 들어오게 될 경우, Rx의 표시가 초록색으로 점등됨으로써 정상적으로 동작하는지에 대한 판단이 가능하다.
