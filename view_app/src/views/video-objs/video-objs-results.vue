<style lang="less">

</style>
<template>
    <div class="video_objs_results_main">
        <h2>视频"{{videoName}}"物体检测结果:</h2>
        <br/>
        <Row>
            <Col span="24" v-for="(objectResults,objectClassID) in results" :key="objectClassID">
                <Card>
                    <p slot="title" v-if="objectClasses[objectClassID]">物体分类ID:{{objectClassID}}&nbsp;&nbsp;名称:{{objectClasses[objectClassID]['name']}}</p>
                    <p slot="title" v-else>物体分类ID:{{objectClassID}}</p>

                    <Row>
                        <Col span="6" v-for="(result,frameID) in objectResults" :key="frameID">
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
    name: 'video_objs_results',
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

        util.ajax.get('/objects/results?video_id='+this.videoID)
            .then((response)=>{
                this.results = {};
                for(let i in response.data) {
                    const result = response.data[i];
                    const objectClassID = result['object_class_id'];
                    
                    if(!this.results[objectClassID]){
                        this.results[objectClassID] = {};
                    }

                    if(!this.results[objectClassID][result['frame_id']]){
                        this.results[objectClassID][result['frame_id']] = [];
                        this.frames[result['frame_id']] = result.second;
                    }

                    this.results[objectClassID][result['frame_id']].push({
                        left:result['left'],
                        top:result['top'],
                        width:result['width'],
                        height:result['height'],
                        prob:result['prob'],
                        createdAt:result['created_at']
                    });
                }
                console.log(this.results); 
            }).catch((error)=>{
                console.log(error);
                alert('无法从后端获取视频检测结果');
            });
        
        util.ajax.get('/objectclasses/lists')
            .then((response)=>{
                this.objectClasses = {};
                for(let i in response.data){
                    const objectClass = response.data[i];
                    
                    if(!this.objectClasses[objectClass.object_class_id]){
                        this.objectClasses[objectClass.object_class_id] = {};
                    }
                    this.objectClasses[objectClass.object_class_id] = {
                        name:objectClass['cn_name'] != '' ? objectClass['cn_name'] : objectClass['en_name']
                    };
                }
                console.log(this.objectClasses);
            }).catch((error)=>{
                console.log(error);
                alert('无法从后端获取物体分类列表');
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
            objectClasses:{},
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
