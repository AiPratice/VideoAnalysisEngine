import Util from '../../libs/util';

const person = {
    state: {
        persons:[]
    },
    mutations: {
        addPerson(state,person){
            state.persons.push(person);
        },
        setPersons(state,persons){
            state.persons = persons;
        }
    },
    actions: {
        getPersons({commit}){
            Util.ajax.get('/persons/list')
                .then((response)=>{
                    commit('setPersons',response.data);
                }).catch((response)=>{
                    alert('获取预标注人物列表失败');
                });
        }
    }
};

export default person;