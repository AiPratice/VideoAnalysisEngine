<style lang="less">

</style>
<template>
    <div class="video_faces_results_main">
        <h2>视频"{{videoName}}"人脸识别结果:</h2>
        <br/>
        <Row>
            <Col span="24" v-for="(faceResults,personID) in results" :key="personID">
                <Card>
                    <p slot="title" v-if="people[personID]">人物ID:{{personID}}&nbsp;&nbsp;名称:{{people[personID]['name']}}</p>
                    <p slot="title" v-else>人物ID:{{personID}}</p>

                    <Row>
                        <Col span="6" v-for="(result,frameID) in faceResults" :key="frameID">
                            <div @click="play(videoID,frames[frameID])">
                                <img v-bind:src="frameImgUrl+frameID" height="100px" class="search_result_item"/>
                            </div>
                        </Col>
                    </Row>
                </Card>
                <br/>
            </Col>
        </Row>

        <!-- 视频播放对话框 -->
        <Modal
            v-model="playerModal"
            title="视频播放"
            @on-ok="playerModalOk"
            @on-cancel="playerModalCancel">
            <video id="videoPlayer" :src="videoUrl+playvideoID" controls="controls" v-if="playerModal" width="100%">
                无法播放视频
            </video>
        </Modal>
    </div>
</template>
<script>
import util from '../../libs/util';
import config from '../../config';
export default {
    name: 'video_faces_results',
    mounted: function(){
        let tempVideoID = parseInt(this.$route.params.video_id);
        if(tempVideoID == this.videoID){
            return;
        }
        this.videoID = tempVideoID;

        util.ajax.get('/videos/find?video_id='+this.videoID)
            .then((response)=>{
                this.videoName = response.data.name;
            }).catch((error)=>{
                console.log(error);
                alert('无法从后端获取视频');
            });

        util.ajax.get('/faces/results?video_id='+this.videoID)
            .then((response)=>{
                this.results = {};
                for(let i in response.data) {
                    const result = response.data[i];
                    const personID = result['person_id'];
                    
                    if(!this.results[personID]){
                        this.results[personID] = {};
                    }

                    if(!this.results[personID][result['frame_id']]){
                        this.results[personID][result['frame_id']] = [];
                        this.frames[result['frame_id']] = result.second;
                    }

                    this.results[personID][result['frame_id']].push({
                        left:result['left'],
                        top:result['top'],
                        width:result['width'],
                        height:result['height'],
                        createdAt:result['created_at']
                    });
                }
                //console.log(this.results); 
            }).catch((error)=>{
                console.log(error);
                alert('无法从后端获取人脸识别结果');
            });
        
        util.ajax.get('/persons/list')
            .then((response)=>{
                this.people = {
                    0:{
                        name:'未知'
                    }
                };
                console.log(response.data);
                for(let i in response.data){
                    const person = response.data[i];
                    
                    if(!this.people[person.person_id]){
                        this.people[person.person_id] = {};
                    }
                    this.people[person.person_id] = {
                        name:person['name']
                    };
                }
                console.log(this.people);
            }).catch((error)=>{
                console.log(error);
                alert('无法从后端获取人物列表');
            });
    },
    computed:{

    },
    data () {
        return {
            videoID:0,
            videoName:'',
            results:{},
            frames:{},
            people:{},
            frameImgUrl:config.HOST+'/frames/frame_img?frame_id=',
            videoUrl:config.HOST+'/videos/video?video_id=',
            playerModal:false, //播放对话框
            playVideoID:0, //被播放的视频id
            playSecond:0 //播放视频的定位秒数
        };
    },
    methods: {
        play(videoID,second){
            console.log(videoID);
            console.log(second);
            //视频播放
            this.playvideoID = videoID;
            this.playSecond = second;
            this.playerModal = true;

            setTimeout(()=>{
                let searchVideoPlayer = document.getElementById('videoPlayer');
                searchVideoPlayer.currentTime = second;
            },300);
        },
        playerModalOk(){
            this.playerModal = false;
        },
        playerModalCancel(){
            this.playerModal = false;
        }
    }    
}
</script>
