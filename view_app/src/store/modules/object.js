import util from '../../libs/util';

const object = {
    state:{
        classes:[] //物体类别
    },
    mutations:{
        setClasses(state, classes){
            state.classes = classes;
        }
    },
    actions:{
        //获取物体分类
        getClasses({commit}){
            util.ajax.get('objectclasses/lists')
                .then((response)=>{
                    commit('setClasses', response.data);
                }).catch((response)=>{
                    alert('获取物体分类失败');
                });
        }
    }
};

export default object;